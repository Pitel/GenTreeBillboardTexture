#ifndef GENTREE_PARAM_H
#define GENTREE_PARAM_H

#include <cstring>
#include <sstream>

typedef struct {
	float r; //delka vektoru
	float theta; //rotace kolem osy Z
	float phi; //odklon od osy Z (rotace kolem Y, aplikovano po rotaci kolem Z)
	operator std::string(){  std::stringstream tmp; tmp << "[r=" << r << ",θ=" << theta << ",φ=" << phi << "]"; return tmp.str(); } //osklive to je, ale je to jen pro debug vypisy
} sphericCoords;

typedef struct {
	float x;
	float y;
	float z;
	operator std::string(){ std::stringstream tmp; tmp << "[" << x << "," << y << "," << z << "]"; return tmp.str(); }
} cartesianCoords;

typedef struct d{
    int level;
	sphericCoords relativeVector; //smer vetve, vzhledem k rodicovske vetvi
	sphericCoords absoluteVector; //smer vetve, vzhledem ke kmeni (presneji k ose Y)
	cartesianCoords branchEnd; //souradnice konce vetve
	float thickness;
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

enum PTreeType {PTREE_1, PTREE_COUNT}; //PTREE_COUNT znaci pocet typu stromu, tedy cislovat postupne a PTREE_COUNT nechat posledni

typedef struct {
	void (*branchLengthFunc)(TreeNode *);
	void (*branchThicknessFunc)(TreeNode *);
	void (*branchDirectionFunc)(TreeNode *);
} treeParams;

//funkce konkretniho stromu
void branchLengthTree1(TreeNode *current);
void branchThicknessTree1(TreeNode *current);
void branchDirectionTree1(TreeNode *current);

//asociace funkci pro parametrizaci ke konkretnimu typu stromu (dle poradi v PTreeType)
extern const treeParams treeBuilders[PTREE_COUNT];

void GenTreeBillboardTexture_parametrize(TreeNode *node, PTreeType treeType, int seed = 0);

void parametrizeNode(TreeNode *node, PTreeType treeType, int level = 0);

#endif
