#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>

const double G = 6.67430e-11;
const double dt = 0.01;
const int STEPS = 100;

struct Body {
    double x, y, z;
    double vx, vy, vz;
    double mass;
};

void compute_force(const Body& a, const Body& b, double& fx, double& fy, double& fz) {
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    double dz = b.z - a.z;
    double epsilon = 1e-3;
    double dist_sqr = dx*dx + dy*dy + dz*dz + epsilon*epsilon;
    double dist = std::sqrt(dist_sqr);
    double force = G * a.mass * b.mass / dist_sqr;
    fx += force * dx / dist;
    fy += force * dy / dist;
    fz += force * dz / dist;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) std::cout << "Starting N-body simulation with " << size << " processes...\n";

    int N = 100;
    std::vector<Body> bodies;
    if (rank == 0) {
        bodies.resize(N);
        for (int i = 0; i < N; ++i) {
            bodies[i] = {rand()%1000, rand()%1000, rand()%1000,
                         0.0, 0.0, 0.0,
                         1e20};
        }
        std::cout << "Process 0 initialized " << N << " bodies.\n";
    }

    if (rank != 0) bodies.resize(N);
    MPI_Bcast(bodies.data(), N * sizeof(Body), MPI_BYTE, 0, MPI_COMM_WORLD);
    std::cout << "Process " << rank << " received initial body data.\n";

    int local_N = N / size;
    int start = rank * local_N;
    int end = (rank == size - 1) ? N : start + local_N;

    double start_time = MPI_Wtime();

    for (int step = 0; step < STEPS; ++step) {
        std::vector<Body> new_bodies = bodies;

        for (int i = start; i < end; ++i) {
            double fx = 0, fy = 0, fz = 0;
            for (int j = 0; j < N; ++j) {
                if (i != j) {
                    compute_force(bodies[i], bodies[j], fx, fy, fz);
                }
            }

            new_bodies[i].vx += fx / bodies[i].mass * dt;
            new_bodies[i].vy += fy / bodies[i].mass * dt;
            new_bodies[i].vz += fz / bodies[i].mass * dt;

            new_bodies[i].x += new_bodies[i].vx * dt;
            new_bodies[i].y += new_bodies[i].vy * dt;
            new_bodies[i].z += new_bodies[i].vz * dt;
        }

        MPI_Allgather(&new_bodies[start], local_N * sizeof(Body), MPI_BYTE,
                      bodies.data(), local_N * sizeof(Body), MPI_BYTE,
                      MPI_COMM_WORLD);

        if (rank == 0 && step % 10 == 0) {
            std::cout << "Completed step " << step << " of " << STEPS << "\n";
        }
    }

    double end_time = MPI_Wtime();
    double elapsed = end_time - start_time;

    std::cout << "Process " << rank << " finished simulation in " << elapsed << " seconds.\n";

    MPI_Finalize();
    return 0;
}
