// ─────────────────────────────────────────────────────────────────────────────
// Predator–Prey Cellular‑Automaton — CPU Reference Implementation + PNG Sprites
//
// Original author : Felice Pantaleo (CERN), 2024
//
// ╭───────────────────────────────  WHAT IS THIS?  ───────────────────────────╮
// │ A toroidal “Game‑of‑Life” variant with three cell states rendered with    │
// │ external PNG tiles. The world evolves in discrete steps on the CPU; a     │
// │ GIF of the whole run can be captured frame‑by‑frame.                      │
// ╰────────────────────────────────────────────────────────────────────────────╯
//
//  Cell states & sprites  (all sprites must be TILE×TILE pixels)              
//    🟩  Empty     – background grass,   file = grass.png                     
//    🐰  Prey      – bunny,             file = bunny.png                     
//    🦊  Predator  – fox,               file = fox.png                       
//
//  Evolution rules (applied every tick to each cell’s 8‑neighbour Moore set)
//  ───────────────────────────────────────────────────────────────────────────
//   Birth
//     • Empty  → Prey       if ≥2 neighbouring preys.
//
//   Death
//     • Prey   → Empty      if exactly 1 stronger predator                 
//                            (pred.level > prey.level‑10).
//     • Prey   → Empty      if overcrowded (>2 neighbouring preys) OR       
//                            no empty neighbour.
//     • Predator → Empty    if no prey neighbours OR every prey is stronger.
//
//   Evolution
//     • Prey   → Predator   if >1 predators AND Σ level(predators) > level(prey).
//
//   Survival bonus
//     • Any surviving Prey/Predator gains +1 level (capped at 255).          
//       Levels influence only the **logic**, not sprite colour.
//          
//
//  Compile‑time switches                                                     
//    SAVE_GRIDS : true  → write simulation.gif (slow & large)                
//                           false → skip GIF, just print runtime.            
//    TILE       : sprite size in pixels (all PNGs **must** match).           
// ─────────────────────────────────────────────────────────────────────────────

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"   // single‑header PNG/JPEG/… loader

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "gif.h"          // Tiny GIF encoder (https://github.com/charlietangora/gif-h)

// ── Compile‑time switches ───────────────────────────────────────────────────
constexpr bool SAVE_GRIDS = true;   // write simulation.gif (slow & disk heavy)
constexpr int  TILE       = 24;     // pixels per automaton cell & sprite size

constexpr char FOX_PNG[]   = "fox.png";   // 24×24 RGBA PNG
constexpr char BUNNY_PNG[] = "bunny.png"; // 24×24 RGBA PNG
constexpr char GRASS_PNG[] = "grass.png"; // 24×24 RGBA PNG

// ── Types ───────────────────────────────────────────────────────────────────
enum class CellState : char { Empty = 0, Predator = 1, Prey = 2 };

struct Cell {
  CellState state;
  uint8_t   level;   // gameplay strength (0‑255) — no longer affects colour
};
using Grid = std::vector<std::vector<Cell>>;   // grid[row][col]

// ── CLI help ────────────────────────────────────────────────────────────────
void print_help() {
  std::cout << "Predator–Prey cellular‑automaton (PNG sprite edition)\n\n";
  std::cout << "PNG tiles required (placed in the current directory):\n"
            << "  • " << FOX_PNG   << "  – predator  (" << TILE << "×" << TILE << ")\n"
            << "  • " << BUNNY_PNG << "  – prey      (" << TILE << "×" << TILE << ")\n"
            << "  • " << GRASS_PNG << "  – grass     (" << TILE << "×" << TILE << ")\n\n";
  std::cout << "Gameplay rules (summary):\n"
            << "  Empty → Prey       when ≥2 neighbouring preys.\n"
            << "  Prey  → Predator   when >1 predators and they are stronger.\n"
            << "  Prey  → Empty      when a single stronger predator exists,\n"
            << "                        or overcrowded, or no empty neighbour.\n"
            << "  Predator → Empty   when no prey, or every prey is stronger.\n"
            << "  Survivors gain +1 level (max 255).\n\n";
  std::cout << "Options:\n"
            << "  --width   <uint>     grid width   (default 200)\n"
            << "  --height  <uint>     grid height  (default 200)\n"
            << "  --weights <empty> <pred> <prey>  spawn weights (ints)\n"
            << "  --seed    <uint>     RNG seed (0 = random)\n"
            << "  --verify  <file>     compare final grid with reference file\n"
            << "  --help              print this help\n\n";
}

// ── World initialisation ────────────────────────────────────────────────────
Grid initialize_grid(size_t width, size_t height,
                     int w_empty, int w_pred, int w_prey,
                     std::mt19937 &gen) {
  Grid g(height, std::vector<Cell>(width));
  std::discrete_distribution<> pick({double(w_empty), double(w_pred), double(w_prey)});
  for (auto &row : g)
    for (auto &c : row) {
      c.state = static_cast<CellState>(pick(gen));
      c.level = (c.state == CellState::Empty) ? 0 : 50;
    }
  return g;
}

// ── Neighbour stats helper ──────────────────────────────────────────────────
struct NeighborData {
  std::vector<uint8_t> predator_levels;
  std::vector<uint8_t> prey_levels;
  uint8_t max_predator_level = 0;
  uint8_t max_prey_level     = 0;
  int     sum_predator_levels = 0;
  int     empty_neighbors     = 0;
};

NeighborData gather_neighbor_data(const Grid &g, int x, int y) {
  NeighborData d;
  const int H = g.size(), W = g[0].size();
  for (int dy = -1; dy <= 1; ++dy)
    for (int dx = -1; dx <= 1; ++dx) {
      if (dx == 0 && dy == 0) continue;            // skip self
      const Cell &n = g[(y + dy + H) % H][(x + dx + W) % W];
      switch (n.state) {
        case CellState::Predator:
          d.predator_levels.push_back(n.level);
          d.max_predator_level = std::max(d.max_predator_level, n.level);
          d.sum_predator_levels += n.level;
          break;
        case CellState::Prey:
          d.prey_levels.push_back(n.level);
          d.max_prey_level = std::max(d.max_prey_level, n.level);
          break;
        case CellState::Empty:
          ++d.empty_neighbors;
          break;
      }
    }
  return d;
}

// ── Game rules update (sequential) ──────────────────────────────────────────
void update_grid_sequential(const Grid &cur, Grid &next) {
  const size_t H = cur.size(), W = cur[0].size();
  for (size_t y = 0; y < H; ++y)
    for (size_t x = 0; x < W; ++x) {
      const Cell &c = cur[y][x];
      Cell &n       = next[y][x];
      const NeighborData nb = gather_neighbor_data(cur, (int)x, (int)y);

      if (c.state == CellState::Empty) {
        n = (nb.prey_levels.size() >= 2)
                ? Cell{CellState::Prey, static_cast<uint8_t>(std::min<int>(nb.max_prey_level + 1, 255))}
                : c;
        continue;
      }

      if (c.state == CellState::Prey) {
        bool done = false;
        if (nb.predator_levels.size() == 1 &&
            nb.predator_levels[0] > (c.level > 10 ? c.level - 10 : 0)) {
          n = {CellState::Empty, 0}; done = true;
        }
        if (!done && nb.prey_levels.size() > 2) { n = {CellState::Empty, 0}; done = true; }
        if (!done && nb.predator_levels.size() > 1 && c.level < nb.sum_predator_levels) {
          n = {CellState::Predator,
                static_cast<uint8_t>(std::min<int>(std::max(nb.max_predator_level, nb.max_prey_level) + 1, 255))};
          done = true;
        }
        if (!done && (nb.empty_neighbors == 0 || nb.prey_levels.size() > 3)) {
          n = {CellState::Empty, 0}; done = true;
        }
        if (!done) {
          n = {CellState::Prey,
                static_cast<uint8_t>((nb.prey_levels.size() < 3 && c.level < 255) ? c.level + 1 : c.level)};
        }
        continue;
      }

      if (c.state == CellState::Predator) {
        if (nb.prey_levels.empty()) {
          n = {CellState::Empty, 0};
        } else {
          bool all_stronger = std::all_of(nb.prey_levels.begin(), nb.prey_levels.end(),
                                           [&](uint8_t l) { return l > c.level; });
          n = all_stronger ? Cell{CellState::Empty, 0}
                           : Cell{CellState::Predator, static_cast<uint8_t>(std::min<int>(c.level + 1, 255))};
        }
      }
    }
}

// ── Sprite loader ───────────────────────────────────────────────────────────
struct Sprite { int w, h; std::vector<uint8_t> rgba; };
Sprite load_png_sprite(const char *file) {
  int w, h, ch; unsigned char *data = stbi_load(file, &w, &h, &ch, 4);
  if (!data) { std::cerr << "Error: cannot load " << file << '\n'; std::exit(1); }
  if (w != TILE || h != TILE) {
    std::cerr << "Error: " << file << " must be " << TILE << " x " << TILE << "\n"; std::exit(1);
  }
  Sprite s{w, h, std::vector<uint8_t>(data, data + 4 * w * h)}; stbi_image_free(data); return s;
}

// ── GIF frame writer ────────────────────────────────────────────────────────
inline void blit_sprite(const Sprite &sp, std::vector<uint8_t> &img, int W, int gx, int gy) {
  const int x0 = gx * TILE, y0 = gy * TILE;
  for (int y = 0; y < TILE; ++y)
    for (int x = 0; x < TILE; ++x) {
      size_t dst = 4 * ((y0 + y) * W + (x0 + x));
      size_t src = 4 * (y * TILE + x);
      std::memcpy(&img[dst], &sp.rgba[src], 4);
    }
}

void save_frame_as_gif(const Grid &g, GifWriter &wr,
                       const Sprite &fox, const Sprite &bunny, const Sprite &grass) {
  if constexpr (!SAVE_GRIDS) return;
  const int cellsW = g[0].size(), cellsH = g.size();
  const int W = cellsW * TILE, H = cellsH * TILE;
  std::vector<uint8_t> img(W * H * 4);
  for (int gy = 0; gy < cellsH; ++gy)
    for (int gx = 0; gx < cellsW; ++gx) {
      switch (g[gy][gx].state) {
        case CellState::Empty:    blit_sprite(grass, img, W, gx, gy); break;
        case CellState::Prey:     blit_sprite(bunny, img, W, gx, gy); break;
        case CellState::Predator: blit_sprite(fox,   img, W, gx, gy); break;
      }
    }
  GifWriteFrame(&wr, img.data(), W, H, 100);   // 100 ms delay per frame
}

// ── Grid I/O for verification ──────────────────────────────────────────────
void save_grid_to_file(const Grid &g, const std::string &fn) {
  std::ofstream o(fn);
  for (auto &r : g) { for (auto &c : r) o << int(c.state) << ' ' << int(c.level) << ' '; o << '\n'; }
}

bool load_grid_from_file(Grid &g, const std::string &fn) {
  std::ifstream i(fn); if (!i) { std::cerr << "Cannot open " << fn << '\n'; return false; }
  for (auto &r : g) for (auto &c : r) { int s, l; i >> s >> l; if (i.fail()) return false; c.state = (CellState)s; c.level = (uint8_t)l; }
  return true;
}

bool compare_grids(const Grid &a, const Grid &b) {
  for (size_t y = 0; y < a.size(); ++y)
    for (size_t x = 0; x < a[0].size(); ++x)
      if (a[y][x].state != b[y][x].state || a[y][x].level != b[y][x].level) return false;
  return true;
}

// ── Main ────────────────────────────────────────────────────────────────────
int main(int argc, char *argv[]) {
  // — Defaults & CLI --------------------------------------------------------
  size_t Wcells = 100, Hcells = 100; unsigned seed = 0; bool seed_set = false;
  int w_e = 5, w_p = 1, w_r = 1; std::string verify_fn;

  for (int i = 1; i < argc; ++i) {
    std::string a = argv[i];
    if (a == "--help") { print_help(); return 0; }
    else if (a == "--seed" && i + 1 < argc) { seed = std::stoul(argv[++i]); seed_set = true; }
    else if (a == "--weights" && i + 3 < argc) { w_e = std::stoi(argv[++i]); w_p = std::stoi(argv[++i]); w_r = std::stoi(argv[++i]); }
    else if (a == "--width" && i + 1 < argc) { Wcells = std::stoul(argv[++i]); }
    else if (a == "--height" && i + 1 < argc) { Hcells = std::stoul(argv[++i]); }
    else if (a == "--verify" && i + 1 < argc) { verify_fn = argv[++i]; }
    else { std::cerr << "Unknown/invalid option " << a << '\n'; return 1; }
  }

  if (!seed_set) seed = std::random_device{}(); std::mt19937 rng(seed);

  // — Load sprites ----------------------------------------------------------
  const Sprite fox   = load_png_sprite(FOX_PNG);
  const Sprite bunny = load_png_sprite(BUNNY_PNG);
  const Sprite grass = load_png_sprite(GRASS_PNG);

  // — Initialise world ------------------------------------------------------
  Grid g = initialize_grid(Wcells, Hcells, w_e, w_p, w_r, rng);
  Grid next = g;

  // — Prepare GIF -----------------------------------------------------------
  GifWriter wr = {};
  if constexpr (SAVE_GRIDS) {
    if (!GifBegin(&wr, "simulation.gif", Wcells * TILE, Hcells * TILE, 50)) {
      std::cerr << "GIF init failed\n"; return 1; }
  }

  // — Simulation loop -------------------------------------------------------
  constexpr size_t ITER = 50;
  const auto t0 = std::chrono::high_resolution_clock::now();
  for (size_t it = 0; it < ITER; ++it) {
    update_grid_sequential(g, next);
    save_frame_as_gif(g, wr, fox, bunny, grass);
    std::swap(g, next);
  }
  const auto t1 = std::chrono::high_resolution_clock::now();
  std::cout << "Sequential elapsed " << std::chrono::duration<double>(t1 - t0).count() << " s\n";
  if constexpr (SAVE_GRIDS) { GifEnd(&wr); std::cout << "Saved simulation.gif\n"; }

  // — Verification or reference write --------------------------------------
  if (!verify_fn.empty()) {
    Grid ref(Hcells, std::vector<Cell>(Wcells));
    if (!load_grid_from_file(ref, verify_fn)) return 1;
    if (compare_grids(g, ref)) {
      std::cout << "Verification OK\n";
    } else {
      std::cerr << "Verification FAILED\n"; return 1;
    }
  } else {
    std::string ref_fn = "reference_" + std::to_string(Wcells) + '_' + std::to_string(Hcells) + '_' +
                         std::to_string(seed) + '_' + std::to_string(w_e) + '_' + std::to_string(w_p) + '_' +
                         std::to_string(w_r) + ".txt";
    save_grid_to_file(g, ref_fn);
    std::cout << "Saved reference grid to " << ref_fn << '\n';
  }
  return 0;
}
