// header file inclusion
#include <windows.h> // windows header files
#include <stdio.h>   // for file IO
#include <stdlib.h>  // for exit
#define _USE_MATH_DEFINES
#include <math.h>

#include <vector>

// openGL headers
#include <gl/GL.h>
#include <gl/GLU.h>

#include "SukrutScene.h"

/*
*******************************************
* Below code is the project related code.
*******************************************
*/

namespace ssd
{
    struct Colour
    {
        float red;
        float green;
        float blue;
        float alpha;

        Colour()
        {
            red = blue = green = 1.0f;
            alpha = 0.0f;
        }

        Colour(float red, float green, float blue, float alpha)
        {
            this->red = red;
            this->green = green;
            this->blue = blue;
            this->alpha = alpha;
        }

        Colour(int red, int green, int blue, float alpha)
        {
            this->red = ((float)red / 255);
            this->green = ((float)green / 255);
            this->blue = ((float)blue / 255);
            this->alpha = alpha;
        }
    };

    struct Vertex
    {
        float x;
        float y;
        float z;

        Colour colour;

        Vertex()
        {
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;

            colour = {1.0f, 1.0f, 1.0f, 1.0f};
        }

        Vertex(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;

            colour = {1.0f, 1.0f, 1.0f, 1.0f};
        }

        void setColour(Colour colour)
        {
            this->colour = std::move(colour);
        }
    };

    struct Translation
    {
        float x;
        float y;
        float z;

        Translation() = delete;

        Translation(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }
    };

    struct Rotation
    {
        float angle;
        float x;
        float y;
        float z;

        Rotation() = delete;

        Rotation(float angle, float x, float y, float z)
        {
            this->angle = angle;
            this->x = x;
            this->y = y;
            this->z = z;
        }
    };

    struct Transformation
    {
        Translation translation;
        Rotation rotation;

        Transformation() = delete;

        Transformation(Translation t, Rotation r) : translation(std::move(t)), rotation(std::move(r))
        {
        }
    };

    struct Ellipse
    {
        Vertex origin;
        float xRadius;
        float yRadius;

        Colour oColour;
        Colour colour;

        Ellipse() = delete;
        Ellipse(Vertex origin, float xRadius, float yRadius)
        {
            this->origin = std::move(origin);
            this->xRadius = xRadius;
            this->yRadius = yRadius;

            oColour = {0.0f, 0.0f, 0.0f, 0.0f};
            colour = {1.0f, 1.0f, 1.0f, 0.0f};
        }

        void setColour(Colour colour)
        {
            this->colour = std::move(colour);
        }

        void draw()
        {
            glBegin(GL_LINES);

            for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.05f)
            {

                float radianAngle = (180.0f) * M_PI * angle;
                float x = xRadius * cos(radianAngle) + origin.x;
                float y = yRadius * sin(radianAngle) + origin.y;

                glColor4f(oColour.red, oColour.green, oColour.blue, oColour.alpha);
                glVertex3f(origin.x, origin.y, origin.z);

                glColor4f(colour.red, colour.green, colour.blue, colour.alpha);
                glVertex3f(x, y, 0.0f);
            }

            glEnd();
        }

        void drawDistanced()
        {
            glBegin(GL_LINES);

            for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.05f)
            {

                float radianAngle = (180.0f) * M_PI * angle;
                float x = xRadius * cos(radianAngle) + origin.x;
                float y = yRadius * sin(radianAngle) + origin.y;

                glColor4f(oColour.red, oColour.green, oColour.blue, oColour.alpha);
                glVertex3f(origin.x, origin.y, origin.z);

                glColor4f(colour.red, colour.green, colour.blue, colour.alpha);
                glVertex3f(x, y, 0.0f);
            }

            glEnd();
        }

        void drawHorizontal()
        {
            glBegin(GL_LINES);

            for (float angle = 0.0f; angle <= 360.0f; angle = angle + 0.1f)
            {

                float radianAngle = (180.0f) * M_PI * angle;
                float x = xRadius * cos(radianAngle) + origin.x;
                float y = yRadius * sin(radianAngle) + origin.y;

                glColor4f(oColour.red, oColour.green, oColour.blue, oColour.alpha);
                glVertex3f(origin.x, origin.y, origin.z);

                glColor4f(colour.red, colour.green, colour.blue, colour.alpha);
                glVertex3f(x, 0.0f, y);
            }

            glEnd();
        }
    };

    struct Polygon
    {
        std::vector<Vertex> vertices;
        Colour colour = {1.0f, 1.0f, 1.0f, 1.0f};
        bool useGlobalColour = false;

        void addVertex(struct Vertex vertex)
        {
            vertices.push_back(std::move(vertex));
        }

        void addVertices(std::vector<Vertex> vertices)
        {
            for (auto &v : vertices)
            {
                this->vertices.push_back(std::move(v));
            }
        }

        void setColour(Colour colour)
        {
            this->colour = std::move(colour);
            useGlobalColour = true;
        }

        void draw()
        {
            glBegin(GL_POLYGON);

            for (const auto &v : vertices)
            {
                if (useGlobalColour)
                {
                    glColor4f(colour.red, colour.green, colour.blue, colour.alpha);
                }
                else
                {
                    glColor4f(v.colour.red, v.colour.green, v.colour.blue, v.colour.alpha);
                }

                glVertex3f(v.x, v.y, v.z);
            }

            glEnd();
        };
    };
}

float angle = 0.0f;
bool fPause = false;

void displayScene()
{
    // function declarations / prototypes / signatures
    void drawFloor();
    void drawTable();
    void drawCpu();
    void drawLamp();
    void drawShelf();
    void drawMonitor();
    void drawChair();
    void SSD_drawGirl();
    void drawWindow();

    // code

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -5.0f);

    // glRotatef(angle, 0.0f, 1.0f, 0.0f);

    drawFloor();

    drawTable();

    drawCpu();
    drawLamp();
    drawShelf();
    drawMonitor();

    SSD_drawGirl();
    drawChair();
    drawWindow();
}

void updateScene()
{
    // code

    if (!fPause)
        angle = angle + 1.0f;
}

void resetModelViewMatrix()
{
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);
}

void drawFloor()
{
    // code
    // glRotatef(angle, 0.0f, 1.0f, 0.0f);

    /*
     * floor
     */
    resetModelViewMatrix();
    glTranslatef(0.0f, 0.0f, -1.0f);
    {
        ssd::Polygon floor;
        floor.addVertices({{-7.0f, -4.0f, 0.0f}, {7.0f, -4.0f, 0.0f}, {7.0f, -1.5f, 0.0f}, {-7.0f, -1.5f, 0.0f}});
        floor.setColour({0.9f, 0.5f, 0.1f, 0.7f});
        floor.draw();
    }

    // skirting
    {
        ssd::Polygon floor;
        floor.addVertices({{-7.0f, -1.5f, 0.0f}, {7.0f, -1.5f, 0.0f}, {7.0f, -1.45f, 0.0f}, {-7.0f, -1.45f, 0.0f}});
        floor.setColour({0.8f, 0.5f, 0.0f, 1.0f});
        floor.draw();
    }
    resetModelViewMatrix();
}

void drawTable()
{
    // code
    // glRotatef(angle, 0.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, -0.1f, -0.05f);
    /*
     * Legs
     */
    {
        // left leg
        ssd::Polygon leg;
        leg.addVertices({{-0.9f, -1.7f, 0.0f}, {-0.82f, -1.7f, 0.0f}, {-0.82f, -0.16f, 0.0f}, {-0.9f, -0.16, 0.0f}});
        leg.setColour({1.0f, 1.0f, 1.0f, 1.0f});
        leg.draw();
    }

    {
        // left leg - 2
        ssd::Polygon leg;
        leg.addVertices({{-0.2f, -1.7f, 0.0f}, {-0.12f, -1.7f, 0.0f}, {-0.12f, -0.16f, 0.0f}, {-0.2f, -0.16, 0.0f}});
        leg.setColour({1.0f, 1.0f, 1.0f, 1.0f});
        leg.draw();
    }

    {
        // left leg between base
        ssd::Polygon leg;
        leg.addVertices({{-0.82f, -1.5f, 0.0f}, {-0.2f, -1.5f, 0.0f}, {-0.2f, -1.3f, 0.0f}, {-0.82f, -1.3, 0.0f}});
        leg.setColour({1.0f, 1.0f, 1.0f, 1.0f});
        leg.draw();
    }

    {
        // right leg
        ssd::Polygon leg;
        leg.addVertices({{2.0f, -1.7f, 0.0f}, {2.1f, -1.7f, 0.0f}, {2.1f, -0.16f, 0.0f}, {2.0f, -0.16, 0.0f}});
        leg.setColour({1.0f, 1.0f, 1.0f, 1.0f});
        leg.draw();
    }

    {
        // right leg - 2
        ssd::Polygon leg;
        leg.addVertices({{3.0f, -1.7f, 0.0f}, {3.1f, -1.7f, 0.0f}, {3.1f, -0.16f, 0.0f}, {3.0f, -0.16, 0.0f}});
        leg.setColour({1.0f, 1.0f, 1.0f, 1.0f});
        leg.draw();
    }

    /*
     * Table top
     */
    {
        // table top
        ssd::Polygon top;
        top.addVertices({{-1.2f, -0.16f, 0.0f}, {3.3f, -0.16f, 0.0f}, {3.3f, -0.06f, 0.0f}, {-1.2f, -0.06, 0.0f}});
        top.setColour({1.0f, 1.0f, 1.0f, 1.0f});
        top.draw();
    }

    /*
     * Table cabinet
     */
    {
        // drawers cabinet
        ssd::Polygon cabinet;
        cabinet.addVertices({{2.1f, -1.3f, 0.0f}, {3.0f, -1.3f, 0.0f}, {3.0f, -0.16f, 0.0f}, {2.1f, -0.16, 0.0f}});
        cabinet.setColour({1.0f, 1.0f, 1.0f, 1.0f});
        cabinet.draw();
    }

    /*
     * Table drawers
     */
    {
        // bottom drawer
        ssd::Polygon drawer;
        drawer.addVertices({{2.1f, -1.3f, 0.0f}, {3.0f, -1.3f, 0.0f}, {3.0f, -1.0f, 0.0f}, {2.1f, -1.0, 0.0f}});
        drawer.setColour({1.0f, 1.0f, 1.0f, 1.0f});
        drawer.draw();

        // left vertical shadow
        ssd::Polygon lvshadow;
        lvshadow.addVertices({{2.1f, -1.2905f, 0.0f}, {2.105f, -1.2905f, 0.0f}, {2.105f, -1.05f, 0.0f}, {2.1f, -1.05, 0.0f}});
        lvshadow.setColour({0.9f, 0.9f, 0.9f, 1.0f});
        lvshadow.draw();

        // right vertical shadow
        ssd::Polygon rvshadow;
        rvshadow.addVertices({{2.995f, -1.2905f, 0.0f}, {3.0f, -1.2905f, 0.0f}, {3.0f, -1.05f, 0.0f}, {2.995f, -1.05, 0.0f}});
        rvshadow.setColour({0.9f, 0.9f, 0.9f, 1.0f});
        rvshadow.draw();

        // handle border
        ssd::Polygon handleBorder;
        handleBorder.addVertices({{2.4f, -1.2f, 0.0f}, {2.7f, -1.2f, 0.0f}, {2.7f, -1.15f, 0.0f}, {2.4f, -1.15, 0.0f}});
        handleBorder.setColour({0.9f, 0.9f, 0.9f, 1.0f});
        handleBorder.draw();

        // handle
        ssd::Polygon handle;
        handle.addVertices({{2.4f, -1.19f, 0.0f}, {2.7f, -1.19f, 0.0f}, {2.7f, -1.16f, 0.0f}, {2.4f, -1.16, 0.0f}});
        handle.setColour({245, 245, 245, 1.0f});
        handle.draw();

        // drawer top shadow
        ssd::Polygon shadow;
        shadow.addVertices({{2.1f, -1.0f, 0.0f}, {3.0f, -1.0f, 0.0f}, {3.0f, -0.95f, 0.0f}, {2.1f, -0.95, 0.0f}});
        shadow.setColour({0.9f, 0.9f, 0.9f, 1.0f});
        shadow.draw();
    }

    {
        // middle drawer
        ssd::Polygon drawer;
        drawer.addVertices({{2.1f, -0.95f, 0.0f}, {3.0f, -0.95f, 0.0f}, {3.0f, -0.65f, 0.0f}, {2.1f, -0.65, 0.0f}});
        drawer.setColour({1.0f, 1.0f, 1.0f, 1.0f});
        drawer.draw();

        // left vertical shadow
        ssd::Polygon lvshadow;
        lvshadow.addVertices({{2.1f, -0.92f, 0.0f}, {2.105f, -0.92f, 0.0f}, {2.105f, -0.68f, 0.0f}, {2.1f, -0.68, 0.0f}});
        lvshadow.setColour({0.9f, 0.9f, 0.9f, 1.0f});
        lvshadow.draw();

        // right vertical shadow
        ssd::Polygon rvshadow;
        rvshadow.addVertices({{2.995f, -0.92f, 0.0f}, {3.0f, -0.92f, 0.0f}, {3.0f, -0.68f, 0.0f}, {2.995f, -0.68, 0.0f}});
        rvshadow.setColour({0.9f, 0.9f, 0.9f, 1.0f});
        rvshadow.draw();

        // handle border
        ssd::Polygon handleBorder;
        handleBorder.addVertices({{2.4f, -0.85f, 0.0f}, {2.7f, -0.85f, 0.0f}, {2.7f, -0.80f, 0.0f}, {2.4f, -0.80, 0.0f}});
        handleBorder.setColour({0.9f, 0.9f, 0.9f, 1.0f});
        handleBorder.draw();

        // handle
        ssd::Polygon handle;
        handle.addVertices({{2.4f, -0.84f, 0.0f}, {2.7f, -0.84f, 0.0f}, {2.7f, -0.81f, 0.0f}, {2.4f, -0.81, 0.0f}});
        handle.setColour({245, 245, 245, 1.0f});
        handle.draw();

        // drawer top shadow
        ssd::Polygon shadow;
        shadow.addVertices({{2.1f, -0.65f, 0.0f}, {3.0f, -0.65f, 0.0f}, {3.0f, -0.60f, 0.0f}, {2.1f, -0.60, 0.0f}});
        shadow.setColour({0.9f, 0.9f, 0.9f, 1.0f});
        shadow.draw();
    }

    {
        // top drawer
        ssd::Polygon drawer;
        drawer.addVertices({{2.1f, -0.60f, 0.0f}, {3.0f, -0.60f, 0.0f}, {3.0f, -0.30f, 0.0f}, {2.1f, -0.30, 0.0f}});
        drawer.setColour({1.0f, 1.0f, 1.0f, 1.0f});
        drawer.draw();

        // left vertical shadow
        ssd::Polygon lvshadow;
        lvshadow.addVertices({{2.1f, -0.58f, 0.0f}, {2.105f, -0.58f, 0.0f}, {2.105f, -0.32f, 0.0f}, {2.1f, -0.32, 0.0f}});
        lvshadow.setColour({0.9f, 0.9f, 0.9f, 1.0f});
        lvshadow.draw();

        // right vertical shadow
        ssd::Polygon rvshadow;
        rvshadow.addVertices({{2.995f, -0.58f, 0.0f}, {3.0f, -0.58f, 0.0f}, {3.0f, -0.32f, 0.0f}, {2.995f, -0.32, 0.0f}});
        rvshadow.setColour({0.9f, 0.9f, 0.9f, 1.0f});
        rvshadow.draw();

        // handle border
        ssd::Polygon handleBorder;
        handleBorder.addVertices({{2.4f, -0.50f, 0.0f}, {2.7f, -0.50f, 0.0f}, {2.7f, -0.45f, 0.0f}, {2.4f, -0.45, 0.0f}});
        handleBorder.setColour({0.9f, 0.9f, 0.9f, 1.0f});
        handleBorder.draw();

        // handle
        ssd::Polygon handle;
        handle.addVertices({{2.4f, -0.49f, 0.0f}, {2.7f, -0.49f, 0.0f}, {2.7f, -0.46f, 0.0f}, {2.4f, -0.46, 0.0f}});
        handle.setColour({245, 245, 245, 1.0f});
        handle.draw();

        // drawer top shadow
        ssd::Polygon shadow;
        shadow.addVertices({{2.0f, -0.28f, 0.0f}, {3.1f, -0.28f, 0.0f}, {3.1f, -0.16f, 0.0f}, {2.0f, -0.16, 0.0f}});
        shadow.setColour({0.9f, 0.9f, 0.9f, 0.9f});
        shadow.draw();
    }

    resetModelViewMatrix();
}

void drawCpu()
{
    // code
    // glRotatef(angle, 0.0f, 1.0f, 0.0f);
    resetModelViewMatrix();
    glTranslatef(0.0f, -0.09f, 0.0f);
    /*
     * cpu
     */
    {
        // cabinet
        ssd::Polygon cabinet;
        cabinet.addVertices({{-0.75f, -1.299f, 0.0f}, {-0.26f, -1.299f, 0.0f}, {-0.26f, -0.24f, 0.0f}, {-0.75f, -0.24, 0.0f}});
        cabinet.setColour({78, 65, 78, 1.0f});
        cabinet.draw();
    }

    {
        // power button panel
        ssd::Polygon left;
        left.addVertices({{-0.60f, -1.2f, 0.0f}, {-0.4f, -1.2f, 0.0f}, {-0.4f, -1.0f, 0.0f}, {-0.60f, -1.0, 0.0f}});
        left.setColour({0.5f, 0.5f, 0.5f, 0.3f});
        left.draw();
    }

    {
        // green button border
        ssd::Ellipse button = {{-0.50f, -1.16f, 0.0f}, 0.04f, 0.02f};
        button.setColour({167, 160, 167, 1.0f});
        button.oColour = {167, 160, 167, 1.0f};
        button.draw();
    }

    {
        // green button
        ssd::Ellipse button = {{-0.50f, -1.16f, 0.0f}, 0.035f, 0.015f};
        button.setColour({154, 205, 50, 0.5f});
        button.oColour = {154, 205, 50, 0.5f};
        button.draw();
    }

    {
        // yellow button border
        ssd::Ellipse button = {{-0.50f, -1.06f, 0.0f}, 0.03f, 0.04f};
        button.setColour({167, 160, 167, 1.0f});
        button.oColour = {167, 160, 167, 1.0f};
        button.draw();
    }
    {
        // yellow button
        ssd::Ellipse button = {{-0.50f, -1.06f, 0.0f}, 0.025f, 0.035f};
        button.setColour({255, 255, 0, 0.5f});
        button.oColour = {255, 255, 0, 0.5f};
        button.draw();
    }

    {
        // top button panel
        ssd::Polygon left;
        left.addVertices({{-0.68f, -0.8f, 0.0f}, {-0.34f, -0.8f, 0.0f}, {-0.34f, -0.4f, 0.0f}, {-0.68f, -0.4, 0.0f}});
        left.setColour({0.5f, 0.5f, 0.5f, 0.3f});
        left.draw();

        ssd::Polygon cd;
        cd.addVertices({{-0.66f, -0.44f, 0.0f}, {-0.36f, -0.44f, 0.0f}, {-0.36f, -0.48f, 0.0f}, {-0.66f, -0.48, 0.0f}});
        cd.setColour({0.5f, 0.5f, 0.5f, 0.8f});
        cd.draw();

        ssd::Polygon one;
        one.addVertices({{-0.66f, -0.54f, 0.0f}, {-0.36f, -0.54f, 0.0f}, {-0.36f, -0.53f, 0.0f}, {-0.66f, -0.53, 0.0f}});
        one.setColour({0.5f, 0.5f, 0.5f, 0.8f});
        one.draw();

        ssd::Polygon two;
        two.addVertices({{-0.66f, -0.62f, 0.0f}, {-0.36f, -0.62f, 0.0f}, {-0.36f, -0.61f, 0.0f}, {-0.66f, -0.61, 0.0f}});
        two.setColour({0.5f, 0.5f, 0.5f, 0.8f});
        two.draw();

        ssd::Polygon three;
        three.addVertices({{-0.66f, -0.70f, 0.0f}, {-0.36f, -0.70f, 0.0f}, {-0.36f, -0.69f, 0.0f}, {-0.66f, -0.69, 0.0f}});
        three.setColour({0.5f, 0.5f, 0.5f, 0.8f});
        three.draw();

        ssd::Polygon four;
        four.addVertices({{-0.66f, -0.78f, 0.0f}, {-0.36f, -0.78f, 0.0f}, {-0.36f, -0.77f, 0.0f}, {-0.66f, -0.77, 0.0f}});
        four.setColour({0.5f, 0.5f, 0.5f, 0.8f});
        four.draw();
    }
    resetModelViewMatrix();
}

void drawLamp()
{
    // code

    // glRotatef(angle, 0.0f, 1.0f, 0.0f);
    resetModelViewMatrix();
    glTranslatef(0.0f, -0.1f, 0.0f);
    /*
     * lamp
     */
    {
        // base
        ssd::Polygon base;
        base.addVertices({{-0.84f, -0.06f, 0.0f}, {-0.64f, -0.06f, 0.0f}, {-0.64f, 0.0f, 0.0f}, {-0.84f, 0.0f, 0.0f}});
        base.setColour({255, 215, 0, 1.0f});
        base.draw();
    }

    {
        // base
        ssd::Polygon base;
        base.addVertices({{-0.77f, 0.0f, 0.0f}, {-0.7f, 0.0f, 0.0f}, {-0.7f, 0.06f, 0.0f}, {-0.77f, 0.06f, 0.0f}});
        base.setColour({255, 215, 0, 1.0f});
        base.draw();
    }

    {
        // base circle-1
        ssd::Ellipse center{{-0.75f, 0.14f, 0.0f}, 0.08f, 0.08f};
        center.setColour({0.8f, 0.7f, 0.5f, 0.5f});
        center.oColour = {0.8f, 0.7f, 0.5f, 0.5f};
        center.draw();

        ssd::Ellipse innerCenter{{-0.75f, 0.14f, 0.0f}, 0.03f, 0.03f};
        innerCenter.colour = {255, 215, 0, 1.0f};
        innerCenter.oColour = {255, 215, 0, 1.0f};
        innerCenter.draw();
    }

    {
        // middle single stick
        ssd::Polygon stick;
        stick.addVertices({{-0.74f, 0.22f, 0.0f}, {-0.78f, 0.22f, 0.0f}, {-0.86f, 0.6f, 0.0f}, {-0.82f, 0.6f, 0.0f}});
        stick.setColour({255, 215, 0, 1.0f});
        stick.draw();
    }

    {
        // base circle-1
        ssd::Ellipse center{{-0.84f, 0.62f, 0.0f}, 0.08f, 0.08f};
        center.setColour({0.8f, 0.7f, 0.5f, 0.5f});
        center.oColour = {0.8f, 0.7f, 0.5f, 0.5f};
        center.draw();

        ssd::Ellipse innerCenter{{-0.84f, 0.62f, 0.0f}, 0.03f, 0.03f};
        innerCenter.colour = {255, 215, 0, 1.0f};
        innerCenter.oColour = {255, 215, 0, 1.0f};
        innerCenter.draw();
    }

    {
        // top double sticks
        ssd::Polygon topStick;
        topStick.addVertices({{-0.84f, 0.70, 0.0f}, {-0.86f, 0.70f, 0.0f}, {-0.60f, 1.0f, 0.0f}, {-0.58f, 1.0f, 0.0f}});
        topStick.setColour({255, 215, 0, 1.0f});
        topStick.draw();

        ssd::Polygon bottomStick;
        bottomStick.addVertices({{-0.80f, 0.69, 0.0f}, {-0.82f, 0.69f, 0.0f}, {-0.58f, 0.98f, 0.0f}, {-0.56f, 0.98f, 0.0f}});
        bottomStick.setColour({255, 215, 0, 1.0f});
        bottomStick.draw();
    }

    {
        // lamp head
        ssd::Polygon head;
        head.addVertices({{-0.52f, 0.74f, 0.0f}, {-0.2f, 0.98f, 0.0f}, {-0.6f, 1.2f, 0.0f}, {-0.62f, 1.18f, 0.0f}});
        head.setColour({255, 215, 0, 1.0f});
        head.draw();
    }

    {
        // lamp light
        ssd::Polygon light;
        light.addVertices({{-0.30f, -0.1f, 0.0f}, {0.8f, -0.1f, 0.0f}, {-0.2f, 0.98f, 0.0f}, {-0.52f, 0.74f, 0.0f}});
        light.setColour({1.0f, 1.0f, 0.6f, 0.2f});
        light.draw();
    }

    resetModelViewMatrix();
}

void drawShelf()
{
    // code

    // glRotatef(angle, 0.0f, 1.0f, 0.0f);

    /*
     * shelf
     */
    {
        // lower horizontal
        ssd::Polygon lower;
        lower.addVertices({{2.2f, 0.9f, 0.0f}, {3.0f, 0.9f, 0.0f}, {3.0f, 0.94f, 0.0f}, {2.2f, 0.94f, 0.0f}});
        lower.setColour({0.7f, 0.4f, 0.0f, 0.8f});
        lower.draw();
    }

    {
        // upper horizontal
        ssd::Polygon upper;
        upper.addVertices({{2.2f, 1.2f, 0.0f}, {3.0f, 1.2f, 0.0f}, {3.0f, 1.24f, 0.0f}, {2.2f, 1.24f, 0.0f}});
        upper.setColour({0.7f, 0.4f, 0.0f, 0.8f});
        upper.draw();
    }

    {
        // left vertical
        ssd::Polygon vertical;
        vertical.addVertices({{2.3f, 0.6f, 0.0f}, {2.35f, 0.6f, 0.0f}, {2.35f, 1.44f, 0.0f}, {2.3f, 1.44f, 0.0f}});
        vertical.setColour({0.7f, 0.4f, 0.0f, 0.8f});
        vertical.draw();
    }

    {
        // right vertical
        ssd::Polygon vertical;
        vertical.addVertices({{2.9f, 0.6f, 0.0f}, {2.85, 0.6f, 0.0f}, {2.85f, 1.44f, 0.0f}, {2.9f, 1.44f, 0.0f}});
        vertical.setColour({0.7f, 0.4f, 0.0f, 0.8f});
        vertical.draw();
    }

    {
        // books
        ssd::Polygon book1;
        book1.addVertices({{2.35f, 0.94f, 0.0f}, {2.45f, 0.94f, 0.0f}, {2.45, 1.15f, 0.0f}, {2.35f, 1.15f, 0.0f}});
        book1.setColour({0.3f, 0.4f, 0.8f, 0.9f});
        book1.draw();

        ssd::Polygon book2;
        book2.addVertices({{2.46f, 0.94f, 0.0f}, {2.56f, 0.94f, 0.0f}, {2.56, 1.15f, 0.0f}, {2.46f, 1.15f, 0.0f}});
        book2.setColour({0.1f, 0.6f, 0.9f, 0.9f});
        book2.draw();

        ssd::Polygon book3;
        book3.addVertices({{2.57f, 0.94f, 0.0f}, {2.67f, 0.94f, 0.0f}, {2.67, 1.15f, 0.0f}, {2.57f, 1.15f, 0.0f}});
        book3.setColour({0.8f, 0.8f, 0.1f, 1.0f});
        book3.draw();

        ssd::Polygon book4;
        book4.addVertices({{2.68f, 0.94f, 0.0f}, {2.78f, 0.94f, 0.0f}, {2.78, 1.15f, 0.0f}, {2.68f, 1.15f, 0.0f}});
        book4.setColour({1.0f, 0.5f, 0.0f, 1.0f});
        book4.draw();
    }
}

void drawMonitor()
{
    // code
    // glRotatef(angle, 0.0f, 1.0f, 0.0f);

    /*
     * monitor
     */
    {
        ssd::Polygon monitorBorder;
        monitorBorder.addVertices({{0.0f, 0.25f, 0.0f}, {1.8f, 0.25f, 0.0f}, {1.8f, 1.2f, 0.0f}, {0.0f, 1.2, 0.0f}});
        monitorBorder.setColour({78, 65, 78, 1.0f});
        monitorBorder.draw();
    }

    {
        ssd::Polygon monitor;
        monitor.addVertices({{0.05f, 0.30f, 0.0f}, {1.75f, 0.30f, 0.0f}, {1.75f, 1.15f, 0.0f}, {0.05f, 1.15, 0.0f}});
        monitor.setColour({5, 5, 5, 0.5f});
        monitor.draw();
    }

    {
        // front
        glBegin(GL_TRIANGLES);

        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.9f, 1.0f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.5f, 0.55f, 0.0f);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(1.3f, 0.55f, 0.0f);

        glEnd();
    }
}

void drawChair()
{
    // code
    // glRotatef(angle, 0.0f, 1.0f, 0.0f);

    /*
     * chair seat
     */
    {
        // cushion
        ssd::Polygon seat;
        seat.addVertices({{0.5f, -0.9f, 0.0f}, {1.4f, -0.9f, 0.0f}, {1.4f, -0.7f, 0.0f}, {0.5f, -0.7, 0.0f}});
        seat.setColour({0.9f, 0.3f, 0.0f, 1.0f});
        seat.draw();
    }

    {
        // cushion support
        ssd::Polygon seat;
        seat.addVertices({{0.7f, -1.0f, 0.0f}, {1.2f, -1.0f, 0.0f}, {1.2f, -0.9f, 0.0f}, {0.7f, -0.9, 0.0f}});
        seat.setColour({0.0f, 0.0f, 0.0f, 1.0f});
        seat.draw();
    }

    {
        // back support cushion
        ssd::Ellipse back = {{0.95f, -0.3f, 0.0f}, 0.25f, 0.32f};
        back.setColour({0.9f, 0.3f, 0.0f, 0.6f});
        back.oColour = {0.9f, 0.3f, 0.0f, 0.6f};
        back.draw();
    }

    {
        // back support
        ssd::Ellipse back = {{0.95f, -0.3f, 0.0f}, 0.2f, 0.3f};
        back.setColour({78, 65, 78, 1.0f});
        back.oColour = {78, 65, 78, 1.0f};
        back.draw();
    }

    {
        // back rod shadow
        ssd::Polygon backRod;
        backRod.addVertices({{0.92f, -1.4f, 0.0f}, {0.98f, -1.4f, 0.0f}, {0.98f, -0.3f, 0.0f}, {0.92f, -0.3, 0.0f}});
        backRod.setColour({0.0f, 0.0f, 0.0f, 0.5f});
        backRod.draw();
    }

    {
        // back rod main
        ssd::Polygon backRod;
        backRod.addVertices({{0.93f, -1.38f, 0.0f}, {0.97f, -1.38f, 0.0f}, {0.97f, -0.32f, 0.0f}, {0.93f, -0.32, 0.0f}});
        backRod.setColour({78, 65, 78, 1.0f});
        backRod.draw();
    }

    {
        // left leg
        ssd::Polygon leg;
        leg.addVertices({{0.92f, -1.3f, 0.0f}, {0.2f, -1.5f, 0.0f}, {0.2f, -1.55f, 0.0f}, {0.92f, -1.4, 0.0f}});
        leg.setColour({78, 65, 78, 1.0f});
        leg.draw();
    }

    {
        // left wheel
        ssd::Ellipse wheel = {{0.2f, -1.6f, 0.0f}, 0.07f, 0.07f};
        wheel.setColour({78, 65, 78, 1.0f});
        wheel.oColour = {78, 65, 78, 1.0f};
        wheel.draw();
    }

    {
        // right leg
        ssd::Polygon leg;
        leg.addVertices({{0.98f, -1.3f, 0.0f}, {1.6f, -1.5f, 0.0f}, {1.6f, -1.55f, 0.0f}, {0.98f, -1.4, 0.0f}});
        leg.setColour({78, 65, 78, 1.0f});
        leg.draw();
    }

    {
        // // right wheel
        ssd::Ellipse wheel = {{1.6f, -1.6f, 0.0f}, 0.07f, 0.07f};
        wheel.setColour({78, 65, 78, 1.0f});
        wheel.oColour = {78, 65, 78, 1.0f};
        wheel.draw();
    }
}

void SSD_drawGirl()
{
    // code

    /*
     *  legs
     */

    {
        // shirt
        ssd::Polygon shirt;
        shirt.addVertices({{0.75f, -0.7f, 0.0f}, {1.1f, -0.7f, 0.0f}, {1.1f, 0.0f, 0.0f}, {0.75f, 0.0f, 0.0f}});
        shirt.setColour({0.6f, 0.2f, 0.4f, 1.0f});
        shirt.draw();
    }

    {
        // left sleeve
        ssd::Polygon shirt;
        shirt.addVertices({{0.75f, -0.2f, 0.0f}, {0.75f, 0.0f, 0.0f}, {0.65f, 0.0f, 0.0f}, {0.65f, -0.2f, 0.0f}});
        shirt.setColour({0.6f, 0.2f, 0.4f, 1.0f});
        shirt.draw();
    }

    {
        // left hand
        ssd::Polygon hand;
        hand.addVertices({{0.65f, -0.15f, 0.0f}, {0.65f, -0.05f, 0.0f}, {0.45f, -0.05f, 0.0f}, {0.45f, -0.15f, 0.0f}});
        hand.setColour({0.9f, 0.7f, 0.5f, 1.0f});
        hand.draw();

        ssd::Polygon hand1;
        hand1.addVertices({{0.45f, -0.15f, 0.0f}, {0.45f, -0.05f, 0.0f}, {0.25f, 0.10f, 0.0f}, {0.18f, 0.08f, 0.0f}});
        hand1.setColour({0.9f, 0.7f, 0.5f, 1.0f});
        hand1.draw();

        resetModelViewMatrix();

        glRotatef(5.0f, 0.0f, 0.0f, 1.0f);
        ssd::Ellipse hand2 = {{0.22f, 0.1f, 0.0f}, 0.05f, 0.08f};
        hand2.setColour({0.9f, 0.7f, 0.5f, 1.0f});
        hand2.oColour = {0.9f, 0.7f, 0.5f, 1.0f};
        hand2.draw();

        ssd::Ellipse hand3 = {{0.22f, 0.20f, 0.0f}, 0.02f, 0.07f};
        hand3.setColour({0.9f, 0.7f, 0.5f, 1.0f});
        hand3.oColour = {0.9f, 0.7f, 0.5f, 1.0f};
        hand3.draw();

        resetModelViewMatrix();

        glRotatef(30.0f, 0.0f, 0.0f, 1.0f);

        ssd::Ellipse hand4 = {{0.23f, 0.09f, 0.0f}, 0.02f, 0.09f};
        hand4.setColour({0.9f, 0.7f, 0.5f, 1.0f});
        hand4.oColour = {0.9f, 0.7f, 0.5f, 1.0f};
        hand4.draw();

        resetModelViewMatrix();
    }

    {
        // right sleeve
        ssd::Polygon shirt;
        shirt.addVertices({{1.1f, -0.2f, 0.0f}, {1.1f, 0.0f, 0.0f}, {1.2f, 0.0f, 0.0f}, {1.2f, -0.2f, 0.0f}});
        shirt.setColour({0.6f, 0.2f, 0.4f, 1.0f});
        shirt.draw();

        ssd::Polygon hand;
        hand.addVertices({{1.2f, -0.05f, 0.0f}, {1.2f, -0.15f, 0.0f}, {1.4f, -0.15f, 0.0f}, {1.4f, -0.05f, 0.0f}});
        hand.setColour({0.9f, 0.7f, 0.5f, 1.0f});
        hand.draw();

        ssd::Polygon hand1;
        hand1.addVertices({{1.4f, -0.15f, 0.0f}, {1.4f, -0.05f, 0.0f}, {1.2f, -0.2f, 0.0f}, {1.2f, -0.3f, 0.0f}});
        hand1.setColour({0.9f, 0.7f, 0.5f, 1.0f});
        hand1.draw();
    }

    {
        ssd::Ellipse head = {{0.9f, 0.2f, 0.0f}, 0.2f, 0.2f};
        head.setColour({0.0f, 0.0f, 0.0f, 1.0f});
        head.oColour = {0.0f, 0.0f, 0.0f, 1.0f};
        head.draw();

        ssd::Ellipse veni1 = {{0.8f, 0.0f, 0.0f}, 0.07f, 0.03f};
        veni1.setColour({0.0f, 0.0f, 0.0f, 1.0f});
        veni1.oColour = {0.0f, 0.0f, 0.0f, 1.0f};
        veni1.draw();
    }

    {
        // left butt
        ssd::Ellipse butt = {{0.94f, -0.65f, 0.0f}, 0.3f, 0.1f};
        butt.setColour({0.4f, 0.3f, 0.8f, 1.0f});
        butt.oColour = {0.4f, 0.3f, 0.8f, 1.0f};
        butt.draw();
    }

    {
        // left leg
        ssd::Ellipse butt = {{0.7f, -1.0f, 0.0f}, 0.1f, 0.4f};
        butt.setColour({0.4f, 0.3f, 0.8f, 1.0f});
        butt.oColour = {0.4f, 0.3f, 0.8f, 1.0f};
        butt.draw();
    }

    {
        // right leg
        ssd::Ellipse butt = {{1.2f, -1.0f, 0.0f}, 0.1f, 0.4f};
        butt.setColour({0.4f, 0.3f, 0.8f, 1.0f});
        butt.oColour = {0.4f, 0.3f, 0.8f, 1.0f};
        butt.draw();
    }
}

void drawWindow()
{
    // code

    {
        // border
        ssd::Polygon window;
        window.addVertices({{-4.2f, 0.2f, 0.0f}, {-2.2f, 0.2f, 0.0f}, {-2.2f, 1.5f, 0.0f}, {-4.2f, 1.5f, 0.0f}});
        window.setColour({0.4f, 0.4f, 0.4f, 2.0f});
        window.draw();

        // white window
        ssd::Polygon window1;
        window1.addVertices({{-4.15f, 0.25f, 0.0f}, {-3.25f, 0.25f, 0.0f}, {-3.25f, 1.45f, 0.0f}, {-4.15f, 1.45f, 0.0f}});
        window1.setColour({25, 25, 112, 1.0f});
        window1.draw();

        ssd::Polygon window2;
        window2.addVertices({{-3.2f, 0.25f, 0.0f}, {-2.25f, 0.25f, 0.0f}, {-2.25f, 1.45f, 0.0f}, {-3.2f, 1.45f, 0.0f}});
        window2.setColour({25, 25, 112, 1.0f});
        window2.draw();
    }

    {
        // moon light
        ssd::Ellipse moon = {{-2.8f, 1.2f, 0.0f}, 0.15f, 0.15f};
        moon.setColour({25, 25, 112, 0.8f});
        moon.oColour = {1.0f, 1.0f, 1.0f, 0.5f};
        moon.draw();

        ssd::Ellipse moon1 = {{-2.8f, 1.2f, 0.0f}, 0.2f, 0.2f};
        moon1.setColour({25, 25, 112, 0.1f});
        moon1.oColour = {1.0f, 1.0f, 1.0f, 1.0f};
        moon1.draw();
    }

    {
        // draw moon
        ssd::Ellipse moon = {{-2.8f, 1.2f, 0.0f}, 0.1f, 0.1f};
        moon.setColour({1.0f, 1.8f, 1.0f, 1.0f});
        moon.oColour = {1.0f, 1.0f, 1.0f, 1.0f};
        moon.draw();


        // moon craters
        ssd::Ellipse moon2 = {{-2.86f, 1.23f, 0.0f}, 0.005f, 0.005f};
        moon2.setColour({220, 220, 220, 0.1f});
        moon2.oColour = {220, 220, 220, 0.1f};
        moon2.draw();

        ssd::Ellipse moon3 = {{-2.87f, 1.19f, 0.0f}, 0.005f, 0.005f};
        moon3.setColour({220, 220, 220, 0.1f});
        moon3.oColour = {220, 220, 220, 0.1f};
        moon3.draw();

        ssd::Ellipse moon4 = {{-2.85f, 1.17f, 0.0f}, 0.005f, 0.005f};
        moon4.setColour({220, 220, 220, 0.1f});
        moon4.oColour = {220, 220, 220, 0.1f};
        moon4.draw();

        ssd::Ellipse moon5 = {{-2.89f, 1.2f, 0.0f}, 0.005f, 0.005f};
        moon5.setColour({220, 220, 220, 0.1f});
        moon5.oColour = {220, 220, 220, 0.1f};
        moon5.draw();
    }

    {
        // moon craters
        ssd::Ellipse moon2 = {{-2.84f, 1.23f, 0.0f}, 0.005f, 0.005f};
        moon2.setColour({220, 220, 220, 0.1f});
        moon2.oColour = {220, 220, 220, 0.1f};
        moon2.draw();

        ssd::Ellipse moon3 = {{-2.83f, 1.19f, 0.0f}, 0.005f, 0.005f};
        moon3.setColour({220, 220, 220, 0.1f});
        moon3.oColour = {220, 220, 220, 0.1f};
        moon3.draw();

        ssd::Ellipse moon4 = {{-2.81f, 1.17f, 0.0f}, 0.005f, 0.005f};
        moon4.setColour({220, 220, 220, 0.1f});
        moon4.oColour = {220, 220, 220, 0.1f};
        moon4.draw();

        ssd::Ellipse moon5 = {{-2.78f, 1.2f, 0.0f}, 0.005f, 0.005f};
        moon5.setColour({220, 220, 220, 0.1f});
        moon5.oColour = {220, 220, 220, 0.1f};
        moon5.draw();
    }

   {
        // stars - window 1
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glPointSize(2.0f);
        glBegin(GL_POINTS);

        glVertex3f(-3.5f, 1.2f, 0.0f);
        glVertex3f(-3.7f, 1.2f, 0.0f);
        glVertex3f(-3.9f, 1.0f, 0.0f);
        glVertex3f(-4.0f, 1.1f, 0.0f);
        glVertex3f(-3.5f, 1.4f, 0.0f);
        glVertex3f(-3.6f, 1.2f, 0.0f);

        glVertex3f(-3.6f, 0.8f, 0.0f);
        glVertex3f(-3.8f, 0.5f, 0.0f);

        glEnd();
   }

      {
        // stars - window 2
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glPointSize(2.0f);
        glBegin(GL_POINTS);

        glVertex3f(-2.5f, 1.2f, 0.0f);
        glVertex3f(-2.7f, 1.2f, 0.0f);
        glVertex3f(-2.9f, 1.2f, 0.0f);
        //glVertex3f(-2.0f, 1.1f, 0.0f);
        glVertex3f(-2.5f, 1.4f, 0.0f);
        glVertex3f(-2.6f, 1.2f, 0.0f);

        glVertex3f(-2.3f, 0.8f, 0.0f);
        glVertex3f(-2.9f, 0.5f, 0.0f);
        glVertex3f(-3.0f, 0.8f, 0.0f);

        glEnd();
   }
}