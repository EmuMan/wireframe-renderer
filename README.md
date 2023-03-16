# wireframe-renderer

A rudimentary interactive wireframe renderer written in C using GLUT.

![Wireframe renderer demo](https://emuman.net/static/images/wireframe-renderer-demo.gif)

## About

This was the final project I wrote for a summer camp I took back in 2017.
The topic was C and rendering pipelines in games, and we were tasked to
create a demonstration using the line rasterizing and matrix multiplication
algorithms we had talked about. I ended up writing a simple wireframe 3D
rendering engine that can read from a custom `.scn` file and display the
objects listed inside in a scene that can be navigated much like a game.

The application itself uses the GLUT framework to render the window, but
the only graphical tool from the library that is used is drawing individual
pixels. All of the line rasterization, 3D transformations and projections,
and culling are done manually. Because of this, even though the project is
very old and has many problems (discussed below), it is still one of my
proudest achievements.

I tried to add a skybox and textures near the end of the project, which
I think is what lead to a lot of the instability discussed below. There
is also likely a lot of bloat and unnecessary inclusions as a result, which
I will filter out when I get the chance.

## Issues

Because I wrote this 6 years ago and didn't fully understand the nuances
of memory at the time, there are probably memory problems (leaks, access
violations, overflows, etc.) all over the place. A real effect of this is
that whether or not the program actually works is basically up to chance.

To maximize this chance, it is best to start the application within Visual
Studio in debug mode. It will naturally be slow (because it is in debug
mode), but it's the only way I've gotten it to eventually work.

When I find the time, I will run back through the code and perform some
fixes and potential optimizations to my past work. I'm sure there are
mountains of points that can be improved on, and I'll update this page when
I get to those.

## Running and Usage

If you do manage to get the program running through the `.sln` file in the
root directory, bring the window into focus and press `v` to allow you to
move the mouse around. From there you *should* be able to use WASD and the
mouse to move around freely (I say "should" because of the aforementioned
problems; sometimes things just don't work).
