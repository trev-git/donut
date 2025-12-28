#include <math.h>
#include <stddef.h>
#include <stdio.h>

#include <raylib.h>

#define size(v) (sizeof((v)) / sizeof((v)[0]))
#define WIDTH 800
#define HEIGHT 800
#define FPS 60

typedef struct Vec3 {
    double x;
    double y;
    double z;
} Vec3;

typedef struct Vec2 {
    double x;
    double y;
} Vec2;

typedef struct Face {
    size_t total_vertices;
    int *face;
} Face;

static const Vec3 vs[] = {
    { 0.25,  0.25,  0.25},
    { 0.25,  0.25, -0.25},
    { 0.25, -0.25,  0.25},
    { 0.25, -0.25, -0.25},
    {-0.25,  0.25,  0.25},
    {-0.25,  0.25, -0.25},
    {-0.25, -0.25,  0.25},
    {-0.25, -0.25, -0.25},
};

static const Face fs[] = {
    {
        .total_vertices = 4,
        .face = (int []){ 0, 2, 6, 4 }
    },
    {
        .total_vertices = 4,
        .face = (int []){ 1, 3, 7, 5 }
    },
    {
        .total_vertices = 2,
        .face = (int []){ 0, 1 }
    },
    {
        .total_vertices = 2,
        .face = (int []){ 2, 3 }
    },
    {
        .total_vertices = 2,
        .face = (int []){ 6, 7 }
    },
    {
        .total_vertices = 2,
        .face = (int []){ 4, 5 }
    }
};

Vec2 project(Vec3 v)
{
    return (Vec2){ v.x / v.z, v.y / v.z };
}

Vec3 translate_x(Vec3 v, double dx)
{
    v.x += dx;
    return v;
}

Vec3 translate_y(Vec3 v, double dy)
{
    v.y += dy;
    return v;
}

Vec3 translate_z(Vec3 v, double dz)
{
    v.z += dz;
    return v;
}

Vec2 screen(Vec2 v)
{
    /* -1..1 -> 0..2 -> 0..1 -> 0..w/h */
    return (Vec2){
        .x = (v.x + 1) / 2 * WIDTH,
        .y = (1 - (v.y + 1)/2) * HEIGHT
    };
}

Vec3 rotate_xz(Vec3 v, double angle)
{
    return (Vec3){
        .x = v.x*cos(angle)-v.z*sin(angle),
        .y = v.y,
        .z = v.x*sin(angle)+v.z*cos(angle)
    };
}

void draw_line(Vec2 v1, Vec2 v2)
{
    DrawLine((int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y, LIME);
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "3d");
    SetTargetFPS(FPS);
    double dx = 0;
    double dy = 0;
    double dz = 1;
    double angle = 0;
    const double dt = 1.0 / FPS;
    char buf[128];

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

        snprintf(buf, 128, "dx: %f, dy: %f, dz: %f", dx, dy, dz);
        angle += M_PI * dt;

        BeginDrawing();
        ClearBackground(BLACK);
        for (unsigned i = 0; i < size(fs); i++)
        {
            for(unsigned j = 0; j < fs[i].total_vertices; j++)
            {
                Vec3 a = vs[fs[i].face[j]];
                Vec3 b = vs[fs[i].face[(j+1) % fs[i].total_vertices]];
                draw_line(screen(project(translate_x(translate_y(translate_z(rotate_xz(a, angle), dz), dy), dx))),
                          screen(project(translate_x(translate_y(translate_z(rotate_xz(b, angle), dz), dy), dx))));
            }
        }
        
        DrawText(buf, 0, 0, 24, WHITE);
        EndDrawing();
    }
}
