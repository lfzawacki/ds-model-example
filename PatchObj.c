/*
    PatchObj - OBJ file converter into OpenGL models
    v. 1.0
    Copyright (C) 2009  Sergio Patricio Figueroa Sanz

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact: Patricio Figueroa <darth.cjous@gmail.com/>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef short int     v16;
typedef short int     v10;
#define  floattov16(n)           ((v16)((n) * (1 << 12)))
#define  floattov10(n)           ((n>.998) ? 0x1FF : ((v10)((n)*(1<<9))))
#define  floattof32(n)           ((int)((n) * (1 << 12)))
#define  NORMAL_PACK(x, y, z)    (((x) & 0x3FF) | (((y) & 0x3FF) << 10) | ((z) << 20))

struct vector{
    double x, y, z;
    struct vector *next;
} *vertex, *texture, *normal;

typedef struct vector VECTOR, * VERTp, * TEXTp, * NORMp;

struct face{
    int points[3][3];
    struct face *next;
} *figure;

char modelname[256];
double max[] = { -9999, -9999, -9999 };
double min[] = {  9999, 9999, 9999 };
float adjust = 1;

typedef struct face FACE, * FACEp;

void newVertex(char *puntos[6]);
void newTexture(char *puntos[6]);
void newNormal(char *puntos[6]);
void makeFace(char *puntos[6]);
void newFace(char *cara[3]);
void dataDump(FILE *file);
void functionDump(FILE *file);
void license(void);

int main(int argc, char *argv[]){
    FILE *arch;
    char line[100], *word[6];
    int count, x, y;
    //license
    license();
    
    if (argc < 3) { printf("I need the modelname too\n"); exit(1); }
    
    if (argc == 5) {
        printf("I did it\n");
        adjust = atof(argv[4]);
    }
    
    strcpy(modelname,argv[3]);
    
    //access obj file
    arch=fopen(argv[1],"r");
    if(arch==NULL){
        printf("Error: file not found or file unavailable\n\n");
        exit(1);
    }
    else{
        while(!feof(arch)){
            //read line
            fgets(line,100,arch);
            //parse tokens
            for(count=0; count<6; count++){
                word[count]=    count==0 ?    strtok(line," ") :    strtok(NULL," ");
                if(word[count]==NULL){
                    word[count-1]=strtok(word[count-1],"\n");
                    break;
                }
            }
            //process data
            if(word[0]!=NULL){
                if(strcmp(word[0],"v")==0){
                    newVertex(word);
                }
                else if(strcmp(word[0],"vt")==0){
                    newTexture(word);
                }
                else if(strcmp(word[0],"vn")==0){
                    newNormal(word);
                }
                else if(strcmp(word[0],"f")==0){
                    makeFace(word);
                }
                else{
                    //skip
                }
            }
        }
        fclose(arch);
        //save to file
        arch=fopen(argv[2],"w");
        if(arch==NULL){
            printf("Error: File creation unsuccessful\n\n");
        }
        else{
            fprintf(arch,"//Generated through PatchObj v.1.0 by Patricio Figueroa\n\n");
            dataDump(arch);
            fclose(arch);
        }
    }
    printf("Success!\n\n");
    return 0;
}

void newVertex(char *puntos[6]){
    VERTp uno, dos;
    if(vertex==NULL){
        vertex=malloc(sizeof(VECTOR));
        vertex->x=puntos[1]==NULL? 0.0 : atof(puntos[1]);
        vertex->y=puntos[2]==NULL? 0.0 : atof(puntos[2]);
        vertex->z=puntos[3]==NULL? 0.0 : atof(puntos[3]);
        vertex->next=NULL;
    }
    else{
        uno=vertex;
        while(uno->next!=NULL){
            dos=uno->next;
            uno=dos;
        }
        uno->next=malloc(sizeof(VECTOR));
        dos=uno->next;
        dos->x=puntos[1]==NULL? 0.0 : atof(puntos[1]);
        dos->y=puntos[2]==NULL? 0.0 : atof(puntos[2]);
        dos->z=puntos[3]==NULL? 0.0 : atof(puntos[3]);    
        dos->next=NULL;    
    }
}

void newTexture(char *puntos[6]){
    TEXTp uno, dos;
    if(texture==NULL){
        texture=malloc(sizeof(VECTOR));
        texture->x=puntos[1]==NULL? 0.0 : atof(puntos[1]);
        texture->y=puntos[2]==NULL? 0.0 : atof(puntos[2]);
        texture->z=puntos[3]==NULL? 0.0 : atof(puntos[3]);
        texture->next=NULL;
    }
    else{
        uno=texture;
        while(uno->next!=NULL){
            dos=uno->next;
            uno=dos;
        }
        uno->next=malloc(sizeof(VECTOR));
        dos=uno->next;
        dos->x=puntos[1]==NULL? 0.0 : atof(puntos[1]);
        dos->y=puntos[2]==NULL? 0.0 : atof(puntos[2]);
        dos->z=puntos[3]==NULL? 0.0 : atof(puntos[3]);    
        dos->next=NULL;    
    }
}

void newNormal(char *puntos[6]){
    NORMp uno, dos;
    if(normal==NULL){
        normal=malloc(sizeof(VECTOR));
        normal->x=puntos[1]==NULL? 0.0 : atof(puntos[1]);
        normal->y=puntos[2]==NULL? 0.0 : atof(puntos[2]);
        normal->z=puntos[3]==NULL? 0.0 : atof(puntos[3]);
        normal->next=NULL;
    }
    else{
        uno=normal;
        while(uno->next!=NULL){
            dos=uno->next;
            uno=dos;
        }
        uno->next=malloc(sizeof(VECTOR));
        dos=uno->next;
        dos->x=puntos[1]==NULL? 0.0 : atof(puntos[1]);
        dos->y=puntos[2]==NULL? 0.0 : atof(puntos[2]);
        dos->z=puntos[3]==NULL? 0.0 : atof(puntos[3]);        
        dos->next=NULL;
    }
}

void makeFace(char *puntos[6]){
    int count=0, x;
    char *points[3], tres[20], uno[20];
    while(puntos[count]!=NULL)    count++;
    count--;
    if(count==3){
        for(x=0; x<3; x++)    points[x]=puntos[x+1];
        newFace(points);
    }
    else if(count==4){
        strcpy(tres,puntos[3]);
        strcpy(uno,puntos[1]);
        for(x=0; x<3; x++)    points[x]=puntos[x+1];
        newFace(points);
        points[0]=tres;
        points[1]=puntos[4];
        points[2]=uno;
        newFace(points);
    }
    else{
        //do nothing
    }
}

void newFace(char *cara[3]){
    FACEp uno, dos;
    char *data[3][3];
    int x, y;
    for(x=0; x<3; x++){
        for(y=0; y<3; y++){
            data[x][y]= y==0? strtok(cara[x],"/") : strtok(NULL,"/");
        }
    }
    if(figure==NULL){
        figure=malloc(sizeof(FACE));
        for(x=0; x<3; x++){
            for(y=0; y<3; y++){
                figure->points[x][y]=data[x][y]==NULL ? 0 : atoi(data[x][y]);
                figure->next=NULL;
            }
        }
    }
    else{
        uno=figure;
        while(uno->next!=NULL){
            dos=uno->next;
            uno=dos;
        }
        uno->next=malloc(sizeof(FACE));
        dos=uno->next;
        for(x=0; x<3; x++){
            for(y=0; y<3; y++){
                dos->points[x][y]=data[x][y]==NULL ? 0 : atoi(data[x][y]);
                dos->next=NULL;
            }
        }
    }
}

void dataDump(FILE *file){
    unsigned int nfaces = 0;

    fprintf(file,"v16 __%s_vertex[][3]={ {0,0,0},\n",modelname);

    while(vertex!=NULL){

        max[0] = vertex->x > max[0] ? vertex->x : max[0];
        max[1] = vertex->y > max[1] ? vertex->y : max[1];
        max[2] = vertex->z > max[2] ? vertex->z : max[2];

        min[0] = vertex->x < min[0] ? vertex->x : min[0];
        min[1] = vertex->y < min[1] ? vertex->y : min[1];
        min[2] = vertex->z < min[2] ? vertex->z : min[2];

        fprintf(file,"    {%d,%d,%d}", floattov16(vertex->x*adjust   ), 
                                     floattov16(vertex->y*adjust    ),
                                     floattov16(vertex->z*adjust   )
                                     );
        vertex=vertex->next;
        if(vertex!=NULL)    fprintf(file,",\n");
    }
    fprintf(file,"};\n\n");

    fprintf(file,"float __%s_max[3] = { %f, %f, %f };\n",modelname,max[0]*adjust,max[1]*adjust,max[2]*adjust);
    fprintf(file,"float __%s_min[3] = { %f, %f, %f };\n\n",modelname,min[0]*adjust,min[1]*adjust,min[2]*adjust);

    fprintf(file,"v16 __%s_texture[][2]={ {0,0},\n",modelname);
    while(texture!=NULL){
        fprintf(file,"    {%d,%d}",floattov16(texture->x),
                                 floattov16(texture->y)
                                 );
        texture=texture->next;
        if(texture!=NULL)    fprintf(file,",\n");
    }
    fprintf(file,"};\n\n");
    fprintf(file,"int __%s_normal[]={ 0,\n",modelname);
    while(normal!=NULL){
        fprintf(file,"    %d", NORMAL_PACK(floattov10(normal->x),
                                            floattov10(normal->y),
                                            floattov10(normal->z))
                                            );
        normal=normal->next;
        if(normal!=NULL)    fprintf(file,",\n");
    }
    fprintf(file,"};\n\n");    
    fprintf(file,"int __%s_face[][3][3]={\n",modelname);
    while(figure!=NULL){
        fprintf(file,"    {{%d,%d,%d},",figure->points[0][0], figure->points[0][1], figure->points[0][2]);
        fprintf(file," {%d,%d,%d},",figure->points[1][0], figure->points[1][1], figure->points[1][2]);
        fprintf(file," {%d,%d,%d}}",figure->points[2][0], figure->points[2][1], figure->points[2][2]);
        figure=figure->next;
        nfaces++;
        if(figure!=NULL)    fprintf(file,",\n");
    }
    fprintf(file,"};\n\n");
    fprintf(file,"unsigned int __%s_nfaces = %u;",modelname,nfaces);
    fflush(file);
}

void license(void){
    char *lic[4]={"   PatchObj  Copyright (C) 2009  Sergio Patricio Figueroa Sanz\n",
            "   This program comes with ABSOLUTELY NO WARRANTY.\n", // for details type `show w'.\n"
        "   This is free software, and you are welcome to redistribute it\n",
        "   under certain conditions.\n" //type `show c' for details.\n"
        };
    int x;
    puts("");
    for(x=0; x<4; x++)
        printf("%s",lic[x]);
    puts("");
}
