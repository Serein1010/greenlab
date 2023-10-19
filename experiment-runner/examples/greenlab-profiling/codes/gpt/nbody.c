#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793
#define SOLAR_MASS (4 * PI * PI)
#define DAYS_PER_YEAR 365.24

typedef struct {
    double x, y, z, vx, vy, vz, mass;
} Body;

Body bodies[5];

void initialize_bodies() {
    bodies[0] = (Body) {
        .mass = SOLAR_MASS
    };

    bodies[1] = (Body) {
        4.84143144246472090e+00,
        -1.16032004402742839e+00,
        -1.03622044471123109e-01,
        1.66007664274403694e-03 * DAYS_PER_YEAR,
        7.69901118419740425e-03 * DAYS_PER_YEAR,
        -6.90460016972063023e-05 * DAYS_PER_YEAR,
        9.54791938424326609e-04 * SOLAR_MASS
    };

    bodies[2] = (Body) {
        8.34336671824457987e+00,
        4.12479856412430479e+00,
        -4.03523417114321381e-01,
        -2.76742510726862411e-03 * DAYS_PER_YEAR,
        4.99852801234917238e-03 * DAYS_PER_YEAR,
        2.30417297573763929e-05 * DAYS_PER_YEAR,
        2.85885980666130812e-04 * SOLAR_MASS
    };

    bodies[3] = (Body) {
        1.28943695621391310e+01,
        -1.51111514016986312e+01,
        -2.23307578892655734e-01,
        2.96460137564761618e-03 * DAYS_PER_YEAR,
        2.37847173959480950e-03 * DAYS_PER_YEAR,
        -2.96589568540237556e-05 * DAYS_PER_YEAR,
        4.36624404335156298e-05 * SOLAR_MASS
    };

    bodies[4] = (Body) {
        1.53796971148509165e+01,
        -2.59193146099879641e+01,
        1.79258772950371181e-01,
        2.68067772490389322e-03 * DAYS_PER_YEAR,
        1.62824170038242295e-03 * DAYS_PER_YEAR,
        -9.51592254519715870e-05 * DAYS_PER_YEAR,
        5.15138902046611451e-05 * SOLAR_MASS
    };
}

void offset_momentum(Body *p, double px, double py, double pz) {
    p->vx = -px / SOLAR_MASS;
    p->vy = -py / SOLAR_MASS;
    p->vz = -pz / SOLAR_MASS;
}

void advance(double dt) {
    for (int i = 0; i < 5; i++) {
        Body *iBody = &bodies[i];
        for (int j = i + 1; j < 5; j++) {
            Body *jBody = &bodies[j];

            double dx = iBody->x - jBody->x;
            double dy = iBody->y - jBody->y;
            double dz = iBody->z - jBody->z;

            double dSquared = dx * dx + dy * dy + dz * dz;
            double distance = sqrt(dSquared);
            double mag = dt / (dSquared * distance);

            iBody->vx -= dx * jBody->mass * mag;
            iBody->vy -= dy * jBody->mass * mag;
            iBody->vz -= dz * jBody->mass * mag;

            jBody->vx += dx * iBody->mass * mag;
            jBody->vy += dy * iBody->mass * mag;
            jBody->vz += dz * iBody->mass * mag;
        }
    }

    for (int i = 0; i < 5; i++) {
        Body *body = &bodies[i];
        body->x += dt * body->vx;
        body->y += dt * body->vy;
        body->z += dt * body->vz;
    }
}

double energy() {
    double e = 0.0;

    for (int i = 0; i < 5; i++) {
        Body *iBody = &bodies[i];
        e += 0.5 * iBody->mass * (iBody->vx * iBody->vx + iBody->vy * iBody->vy + iBody->vz * iBody->vz);

        for (int j = i + 1; j < 5; j++) {
            Body *jBody = &bodies[j];
            double dx = iBody->x - jBody->x;
            double dy = iBody->y - jBody->y;
            double dz = iBody->z - jBody->z;
            double distance = sqrt(dx*dx + dy*dy + dz*dz);
            e -= (iBody->mass * jBody->mass) / distance;
        }
    }
    return e;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <iterations>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);

    initialize_bodies();

    double px = 0.0, py = 0.0, pz = 0.0;
    for (int i = 0; i < 5; i++) {
        px += bodies[i].vx * bodies[i].mass;
        py += bodies[i].vy * bodies[i].mass;
        pz += bodies[i].vz * bodies[i].mass;
    }
    offset_momentum(&bodies[0], px, py, pz);

    printf("%.9f\n", energy());
    for (int i = 0; i < n; i++) {
        advance(0.01);
    }
    printf("%.9f\n", energy());
    return 0;
}
