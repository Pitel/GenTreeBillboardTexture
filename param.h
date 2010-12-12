#ifndef GENTREE_PARAM_H
#define GENTREE_PARAM_H

#include <cstring>
#include <sstream>
#include <queue>
#include <cmath>

/** Vyctovy typ moznych zpusobu vizualizace vetve. Lze vizualizovat barvou dreva, nebo barvou listu */
enum leafVisType {LVIS_WOOD, LVIS_LEAF};

/** Struktura pro uchovani souradnic ve sferickem souradnem systemu */
typedef struct {
	float r; ///< delka vektoru
	float theta; ///< odklon od osy Z (rotace kolem Y, aplikovano po rotaci kolem Z)
	float phi; ///< rotace kolem osy Z
	operator std::string(){  std::stringstream tmp; tmp << "[r=" << r << ",θ=" << theta << ",φ=" << phi << "]"; return tmp.str(); } //osklive to je, ale je to jen pro debug vypisy
} sphericCoords;

/** Struktura pro uchovani souradnic v kartezskem souradnem systemu */
typedef struct cartesianCoords {
	float x;
	float y;
	float z;
	operator std::string(){ std::stringstream tmp; tmp << "[" << x << "," << y << "," << z << "]"; return tmp.str(); }
	cartesianCoords(){ x=0.0f; y=0.0f; z=0.0f; }
	/** Pomocna funkce na vypocet vzdalenosti 2 bodu
	 * @param target Bod, od nehoz se pocita vzdalenost
	 */
	float distanceFrom(cartesianCoords target){
		double xd = this->x-target.x;
		double yd = this->y-target.y;
		double zd = this->z-target.z;
		return sqrt(xd*xd + yd*yd + zd*zd);
	}
} cartesianCoords;

/** Struktura uchovavajici informace o konkretni vetvi vygenerovaneho stromu (uvazujeme strom jako graf) */
typedef struct ParamInfo {
    int level; ///< Uroven zanoreni dane vetve
	sphericCoords relativeVector; ///< smer vetve, vzhledem k rodicovske vetvi
	sphericCoords absoluteVector; ///< smer vetve, vzhledem ke kmeni (presneji k ose Y)
	cartesianCoords branchEnd; ///< souradnice konce vetve
	float thickness; ///< Tloustka dane vetve
	float leafs; ///< nakolik je vetev zalistena, tedy prumerny pocet listu pripadajici na usek vetve o delce leafsize
	float leafsize; ///< velikost listu
	leafVisType branchvis; ///< zpusob vizualizace vetve
	int childLeafs; ///< kolik listu grafu se vyskytuje dohromady ve vsech detech uzlu
	operator std::string(){ std::stringstream tmp; tmp << "{l: " << level << ", rV: " << (std::string)relativeVector << ", aV: " << (std::string)absoluteVector << ", bE: " << (std::string)branchEnd << "}"; return tmp.str(); }
} paramInfo;

#include "grammar.h" //nejprv je nutne nadefinovat structy vyse, pak az lze icnludovat grammar

//funkce pro rand (prejaty z IMS projektu)

/**
 * Vrati nahodne cislo v intervalu <0,1), neresi srand
 */
double basicRandom();

/** Nahodne cislo s exponencialnim rozlozenim
 * @param mv Stredni hodnota generovanych cisel
 */

double exponentialRandom(double mv);

/** Nahodne cislo s normalnim (Gaussovym) rozlozenim
 * @param mean Stredni hodnota generovanych cisel
 * @param var Stredni odchylka generovanych cisel
 */
double normalRandom(double mean, double var);

/** Nahodne cislo s uniformnim (rovnomernym) rozlozenim
 * @param l Spodni hranice generovanych cisel
 * @param h Horni hranice generovanych cisel, musi platit h>l
 */
double uniformRandom(double l, double h);

/** Funkce po doplneni potrebnych udaju - spocita absolutni vektor smeru vetve a koncove souradnice.
 * Predpoklada, ze relativni vektor je jiz vyplnen (z nej a param.branchEnd rodice pocita absolutni vektor).
 * Funkce spocita i branchEnd aktualni vetve.
 */
void fillAbsoluteVector(TreeNode *current);

/**
 * Funkce dopocita branchEnd na zaklade absolutniho vektoru aktualni vetve a param.branchEnd rodice.
 */
void fillBranchEnd(TreeNode *current);

/**
 * Struktura uchovavajici ukazatele na funkce, ktere jsou volany pri generovani konkretniho typu stromu
 */
typedef struct {
	void (*branchLengthFunc)(TreeNode *, int); ///< funkce pro dosazeni delky vetve
	void (*branchThicknessFunc)(TreeNode *, int); ///< funkce pro dosazeni tloustky vetve
	void (*branchDirectionFunc)(TreeNode *, cartesianCoords); ///< funkce pro dosazeni smeru vetve
} treeParams;

/** Struktura pro uchovani rozsahu stromu, tedy bounding boxu */
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

// jablon
void branchLengthTree1(TreeNode *current, int maxlevel);
void branchThicknessTree1(TreeNode *current, int maxlevel);
void branchDirectionTree1(TreeNode *current, cartesianCoords treetopCenter);

// smrk
void branchLengthPicea(TreeNode *current, int maxlevel);
void branchThicknessPicea(TreeNode *current, int maxlevel);
void branchDirectionPicea(TreeNode *current, cartesianCoords treetopCenter);

// palma
void branchLengthPalma(TreeNode *current, int maxlevel);
void branchThicknessPalma(TreeNode *current, int maxlevel);
void branchDirectionPalma(TreeNode *current, cartesianCoords treetopCenter);

/// asociace funkci pro parametrizaci ke konkretnimu typu stromu (dle poradi v PTreeType)
extern const treeParams treeBuilders[PTREE_COUNT];

/** Funkce pro parametrizaci konkretniho stromu vygenerovaneho gramatikou
 * @param node Ukazatel na korenovou vetev stromu
 * @param treeType Typ stromu, jeho psrislusne funkce budou pri parametrizaci volany
 * @param seed Seed, kterym je inicializovan srand
 */
void GenTreeBillboardTexture_parametrize(TreeNode *node, PTreeType treeType, int seed = 0);

/** Funkce pro parametrizaci vetvi
 * @param processNodes Fronta vetvi ke zpracovani. Predpoklada se, ze maji stejny level.
 * @param treeType Typ stromu, jeho psrislusne funkce budou pri parametrizaci volany
 * @param maxlevel Maximalni uroven zanoreni grafu
 * @param treetopCenter Bod urcujici aktualni stred koruny stromu, je prubezne pocitan
 * @param treetopBranchCount Pocet vetvi v aktualni korune stromu, nutne pro prubezny vypocet stredu koruny
 */
void parametrizeNodes(queue<TreeNode*> *processNodes, PTreeType treeType, int maxlevel, cartesianCoords treetopCenter, int treetopBranchCount);

/** Funkce pro parametrizaci konkretni vetve
 * @see parametrizeNodes
 */
void parametrizeNode(TreeNode *node, PTreeType treeType, int maxlevel, cartesianCoords treetopCenter);

/** Funkce pro vypocet bounding boxu stromu
 * @param node Korenova vetev stromu
 */
boundingBox getBoundingBox(TreeNode *node);

/** Funkce pro kombinaci bounding boxu
 * @return Vraci Pro kazdou osu vetsi z maxim a mensi z minim obou bounding boxu.
 */
boundingBox combineBoundingBoxes(boundingBox bb1, boundingBox bb2);

/// zjistni prvniho bodu stromu a jeho pripadne nastaveni na zadouci hodnoty
cartesianCoords getAndResetTreeBase(TreeNode *parentNode);

/// Vypocet maximalniho zanoreni stromu
int maxLevel(TreeNode *node);
/// Vypocet a ulozeni pocetu potomku dane vetve
int calcChildLeafs(TreeNode *node);
#endif
