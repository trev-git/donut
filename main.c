#include <string.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#ifdef DEBUG
#include <time.h>
#endif

#include <raylib.h>

#define WIDTH 800
#define HEIGHT 800
#define FPS 60

typedef struct Vec3 {
    double x;
    double y;
    double z;
} Vec3;

static const double R = 0.25;
static const double r = 0.125;

static Color buffer[WIDTH * HEIGHT];

#ifdef DEBUG
struct timespec get_diff(struct timespec start, struct timespec end)
{
    struct timespec ret;
    ret.tv_sec = end.tv_sec - start.tv_sec;
    if (start.tv_nsec > end.tv_nsec)
    {
        ret.tv_sec--;
        ret.tv_nsec = start.tv_nsec - end.tv_nsec;
    }
    else
    {
        ret.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return ret;
}
#endif

int main()
{
    InitWindow(WIDTH, HEIGHT, "3d");
    SetTargetFPS(FPS);

    double dx = 0;
    double dy = 0;
    double dz = 1;
    double angle = 0;
    double x0, y0, z0;
    const double dt = 1.0 / FPS;

    Image img = {
        .data = buffer,
        .width = WIDTH,
        .height = HEIGHT,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };
    Texture2D texture = LoadTextureFromImage(img);

    char text[128];

    while(!WindowShouldClose())
    {
        if (IsKeyDown(KEY_W))
        {
            dz -= dt;
        }
        if (IsKeyDown(KEY_A))
        {
            dx += dt;
        }
        if (IsKeyDown(KEY_S))
        {
            dz += dt;
        }
        if (IsKeyDown(KEY_D))
        {
            dx -= dt;
        }
        if (IsKeyDown(KEY_LEFT_SHIFT))
        {
            dy += dt;
        }
        if (IsKeyDown(KEY_SPACE))
        {
            dy -= dt;
        }

        snprintf(text, 128, "dx: %f, dy: %f, dz: %f", dx, dy, dz);
        angle += M_PI_2 * dt;
        Vec3 c;
        double ca = cos(angle);
        double sa = sin(angle);

#ifdef DEBUG
        struct timespec st = {0};
        clock_gettime(CLOCK_MONOTONIC, &st);
#endif

        for (double psi = 0; psi <= 2.0*M_PI; psi += 0.05)
        {
            double cos_psi = cos(psi);
            double sin_psi = sin(psi);
            for(double phi = 0; phi <= 2.0*M_PI; phi += 0.01)
            {
                c.x = (R+r*cos_psi)*cos(phi);
                c.y = r*sin_psi;
                c.z = (R+r*cos_psi)*sin(phi);

                // rotate around x
                y0 = c.y * ca - c.z * sa;
                z0 = c.y * sa + c.z * ca;
                c.y = y0;
                c.z = z0;

                // rotate around z
                x0 = c.x * ca - c.y * sa;
                y0 = c.x * sa + c.y * ca;
                c.x = x0;
                c.y = y0;

                // translate
                c.x += dx;
                c.y += dy;
                c.z += dz;

                if (c.z <= 0) continue;

                // project
                c.x /= c.z;
                c.y /= c.z;

                int sx = (c.x + 1) / 2 * WIDTH;
                int sy = (1 - (c.y + 1) / 2) * HEIGHT;
                if ((unsigned int)sx > WIDTH || (unsigned int)sy > HEIGHT) continue;
                buffer[sy*WIDTH + sx] = LIME;
            }
        }

#ifdef DEBUG
        struct timespec end;
        clock_gettime(CLOCK_MONOTONIC, &end);
        struct timespec delta = get_diff(st, end);
        printf("%ld.%09ldns\r", delta.tv_sec, delta.tv_nsec);
#endif

        UpdateTexture(texture, buffer);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(texture, 0, 0, WHITE);
        DrawText(text, 0, 0, 24, WHITE);
        EndDrawing();
        memset(buffer, 0, WIDTH*HEIGHT*sizeof(Color));
    }
}
