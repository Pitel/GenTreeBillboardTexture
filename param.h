#ifndef GENTREE_PARAM_H
#define GENTREE_PARAM_H

#include <cstring>
#include <sstream>
#include <queue>
#include <cmath>

typedef struct {
	float r; //delka vektoru
	float theta; //odklon od osy Z (rotace kolem Y, aplikovano po rotaci kolem Z)
	float phi; //rotace kolem osy Z
	operator std::string(){  std::stringstream tmp; tmp << "[r=" << r << ",θ=" << theta << ",φ=" << phi << "]"; return tmp.str(); } //osklive to je, ale je to jen pro debug vypisy
} sphericCoords;

typedef struct cartesianCoords {
	float x;
	float y;
	float z;
	operator std::string(){ std::stringstream tmp; tmp << "[" << x << "," << y << "," << z << "]"; return tmp.str(); }
	cartesianCoords(){ x=0.0f; y=0.0f; z=0.0f; }
	float distanceFrom(cartesianCoords target){
		double xd = this->x-target.x;
		double yd = this->y-target.y;
		double zd = this->z-target.z;
		return sqrt(xd*xd + yd*yd + zd*zd);
	}
} cartesianCoords;

typedef struct d{
    int level;
	sphericCoords relativeVector; //smer vetve, vzhledem k rodicovske vetvi
	sphericCoords absoluteVector; //smer vetve, vzhledem ke kmeni (presneji k ose Y)
	cartesianCoords branchEnd; //souradnice konce vetve
	float thickness;
	float leafs; //nakolik je vetev zalistena - <0, 1>
	float leafsize; //velikost listu
	int childLeafs; //kolik listu grafu se vyskytuje dohromady ve vsech detech uzlu
	operator std::string(){ std::stringstream tmp; tmp << "{l: " << level << ", rV: " << (std::string)relativeVector << ", aV: " << (std::string)absoluteVector << ", bE: " << (std::string)branchEnd << "}"; return tmp.str(); }
} paramInfo;

#include "grammar.h" //nejprv je nutne nadefinovat structy vyse, pak az lze icnludovat grammar

//funkce pro rand (prejaty z IMS projektu)

//rand <0, 1) (predpoklada inicializovany srand)
double basicRandom();

//exponencialni rozdeleni
//mv = mean value
double exponentialRandom(double mv);

//normalni rozdeleni
//mean = mean value, var = variability
double normalRandom(double mean, double var);

//uniformni rozdeleni
//l < h
double uniformRandom(double l, double h);

//funkce po doplneni potrebnych udaju spocita absolutni vektor smeru vetve a koncove souradnice
void fillAbsoluteVector(TreeNode *current);

enum PTreeType {PTREE_1, PTREE_PICEA, PTREE_COUNT}; //PTREE_COUNT znaci pocet typu stromu, tedy cislovat postupne a PTREE_COUNT nechat posledni

typedef struct {
	void (*branchLengthFunc)(TreeNode *, int);
	void (*branchThicknessFunc)(TreeNode *, int);
	void (*branchDirectionFunc)(TreeNode *, cartesianCoords);
} treeParams;

typedef struct {
	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;
	operator std::string(){ std::stringstream tmp; tmp << "{x: [" << minX << "," << maxX << "], y: [" << minY << "," << maxY << "], z: [" << minZ << "," << maxZ << "]}"; return tmp.str(); }
} boundingBox;

//funkce konkretniho stromu
void branchLengthTree1(TreeNode *current, int maxlevel);
void branchThicknessTree1(TreeNode *current, int maxlevel);
void branchDirectionTree1(TreeNode *current, cartesianCoords treetopCenter);

void branchLengthPicea(TreeNode *current, int maxlevel);
void branchThicknessPicea(TreeNode *current, int maxlevel);
void branchDirectionPicea(TreeNode *current, cartesianCoords treetopCenter);

//asociace funkci pro parametrizaci ke konkretnimu typu stromu (dle poradi v PTreeType)
extern const treeParams treeBuilders[PTREE_COUNT];

void GenTreeBillboardTexture_parametrize(TreeNode *node, PTreeType treeType, int seed = 0);

void parametrizeNodes(queue<TreeNode*> *processNodes, PTreeType treeType, int maxlevel, cartesianCoords treetopCenter, int treetopBranchCount);
void parametrizeNode(TreeNode *node, PTreeType treeType, int maxlevel, cartesianCoords treetopCenter);
boundingBox getBoundingBox(TreeNode *node);
boundingBox combineBoundingBoxes(boundingBox bb1, boundingBox bb2);

int maxLevel(TreeNode *node);
int calcChildLeafs(TreeNode *node);
#endif
