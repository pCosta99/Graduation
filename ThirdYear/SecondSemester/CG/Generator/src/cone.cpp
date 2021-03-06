#include "../headers/cone.h"
#include <math.h>

Cone::Cone(float radius, float height, int slices, int stacks){
    this->radius = radius;
    this->height = height;
    this->slices = slices;
    this->stacks = stacks;

    genVertices();
}

void Cone::drawButtom(){
    float dx = (float)(2 * M_PI) / (float)slices;
    float alpha = 0;
    float tx = 1 / ceil(slices / 2);

    for(int i = 0; i < slices; i++){
        addVertice(new Vertice(radius * sin(alpha),0,radius * cos(alpha)));
        addVertice(new Vertice(0,0,0));
        addVertice(new Vertice(radius * sin(alpha + dx),0,radius * cos(alpha+dx)));

        addNormal(new Vertice(0.0f,-1.0f,0.0));
        addNormal(new Vertice(0.0f,-1.0f,0.0));
        addNormal(new Vertice(0.0f,-1.0f,0.0));

        addTexture(new Vertice(0.5 * sin(alpha) + 0.5, 0.5 * cos(alpha) + 0.5, 0));
        addTexture(new Vertice(0.5,0.5,0));
        addTexture(new Vertice(0.5 * sin(alpha + dx) + 0.5, 0.5 * cos(alpha + dx) + 0.5, 0));

        alpha += dx;
    }
}

void Cone::drawFaces(){
    float h = height / stacks;
    float hi,hs;
    float ri, rs;
    float alpha;
    float dx = (float)(2 * M_PI) / (float)slices;
    float x1,x2,x3,x4,z1,z2,z3,z4;

    float nx, nz, ny = cos(atan(h / radius));

    for(int i = 0; i < stacks; i++){
        hi = h * (float)i;
        hs = h * (float)(i+1);
        ri = radius - ((radius * hi) / height);
        rs = radius - ((radius * hs) / height);
        for(int j = 0; j < slices; j++){
            alpha = dx * j;
            
            x1 = ri * sin(alpha);
            x2 = ri * sin(alpha + dx);
            x3 = rs * sin(alpha);
            x4 = rs * sin(alpha + dx);

            z1 = ri * cos(alpha);
            z2 = ri * cos(alpha + dx);
            z3 = rs * cos(alpha);
            z4 = rs * cos(alpha + dx);

            addVertice(new Vertice(x1,hi,z1));
            addVertice(new Vertice(x2,hi,z2));
            addVertice(new Vertice(x3,hs,z3));

            addVertice(new Vertice(x2,hi,z2));
            addVertice(new Vertice(x4,hs,z4));
            addVertice(new Vertice(x3,hs,z3));

            addNormal(new Vertice(sin(alpha),ny,cos(alpha)));
            addNormal(new Vertice(sin(alpha+dx),ny,cos(alpha+dx)));
            addNormal(new Vertice(sin(alpha),ny,cos(alpha)));

            addNormal(new Vertice(sin(alpha+dx),ny,cos(alpha+dx)));
            addNormal(new Vertice(sin(alpha+dx),ny,cos(alpha+dx)));
            addNormal(new Vertice(sin(alpha),ny,cos(alpha)));
            
            addTexture(new Vertice((float)j / slices,(float)i / stacks,0));
            addTexture(new Vertice((float)(j+1)/slices,(float)i / stacks,0));
            addTexture(new Vertice((float)j / slices,(float)(i+1) / stacks,0));

            addTexture(new Vertice((float)(j+1)/slices,(float)i / stacks,0));
            addTexture(new Vertice((float)(j+1)/slices,(float)(i+1) / stacks,0));
            addTexture(new Vertice((float)j / slices,(float)(i+1) / stacks,0));

        }
    }
}

void Cone::genVertices(){
    drawButtom();
    drawFaces();
}
