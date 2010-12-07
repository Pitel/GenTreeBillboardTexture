#include <stdio.h>
#include <stdlib.h>
#include "param.h"
#include <cmath>
#include <queue>

const treeParams treeBuilders[PTREE_COUNT] = {
   /* PTREE_APPLE */ {branchLengthTree1, branchThicknessTree1, branchDirectionTree1},
   /* PTREE_PICEA */ {branchLengthPicea, branchThicknessPicea, branchDirectionPicea}
};

void GenTreeBillboardTexture_parametrize(TreeNode *tree, PTreeType treeType, int seed) { //oparamerizuje dany strom
	srand(seed); //zinicializujeme rand
	int maxlvl = maxLevel(tree);
	calcChildLeafs(tree); //nechame spocitat pocty listu v kazde vetvi
	queue<TreeNode *> processNodes;
	processNodes.push(tree);
	tree->param.level = 1;
	cartesianCoords treetopCenter;
	parametrizeNodes(&processNodes, treeType, maxlvl, treetopCenter, 0);
	return;
}

void parametrizeNodes(queue<TreeNode*> *processNodes, PTreeType treeType, int maxlevel, cartesianCoords treetopCenter, int treetopBranchCount) {
	int size = processNodes->size();
	cartesianCoords newTreetopCenter = treetopCenter;
	newTreetopCenter.x *= treetopBranchCount;
	newTreetopCenter.y *= treetopBranchCount;
	newTreetopCenter.z *= treetopBranchCount;
	for(int i = 0;i < size;i++) {
		TreeNode* node = processNodes->front();
		processNodes->pop();

		//nechame oparatemtrizovat
		parametrizeNode(node, treeType, maxlevel, treetopCenter);

		//nejake vypocty co se koruny tyce
		treetopBranchCount++; //navysime pocet vetvi v korune
		if(node->parentNode != NULL){
			newTreetopCenter.x += (node->parentNode->param.branchEnd.x+node->param.branchEnd.x)/2.0;
			newTreetopCenter.y += (node->parentNode->param.branchEnd.y+node->param.branchEnd.y)/2.0;
			newTreetopCenter.z += (node->parentNode->param.branchEnd.z+node->param.branchEnd.z)/2.0;
		}else{
			newTreetopCenter.x += node->param.branchEnd.x/2.0;
			newTreetopCenter.y += node->param.branchEnd.y/2.0;
			newTreetopCenter.z += node->param.branchEnd.z/2.0;
		}

		//pridame ke zpracovani vsechny potomky
		for(unsigned int j=0;j<node->childNodes.size();j++) {
			node->childNodes[j]->param.level = node->param.level+1;
			processNodes->push(node->childNodes[j]);
		}
	}

	newTreetopCenter.x /= treetopBranchCount;
	newTreetopCenter.y /= treetopBranchCount;
	newTreetopCenter.z /= treetopBranchCount;

	if(processNodes->size() != 0) {
		parametrizeNodes(processNodes, treeType, maxlevel, newTreetopCenter, treetopBranchCount);
	}
}
void parametrizeNode(TreeNode *node, PTreeType treeType, int maxlevel, cartesianCoords treetopCenter){
	//parametrizace daneho uzlu
	//zavolame prislusne funkce parametrizujici v danem typu stromu
	treeBuilders[treeType].branchLengthFunc(node, maxlevel);
	treeBuilders[treeType].branchThicknessFunc(node, maxlevel);
	treeBuilders[treeType].branchDirectionFunc(node, treetopCenter);

	//pote nechame projit vsechny potomky
	if(node->childNodes.size() == 0){
		node->param.leafs = 0.8; //koncova vetev, dame ji nejake listy
	}else{
		node->param.leafs = 0.0;
	}
}

void branchLengthTree1(TreeNode *current, int maxlevel) {
	//delku vetve spocitame z levelu
	current->param.relativeVector.r = 0.04*uniformRandom(0.7, 1.3)*(1.0+maxlevel)/current->param.level;
}
void branchThicknessTree1(TreeNode *current, int maxlevel) {
	current->param.thickness = sqrt(current->param.childLeafs+1.0)*0.0015; //tloustka vetve odpovida poctu vetvi, ktere z teto rostou (musi sedet plocha prurezu)
}

void branchDirectionTree1(TreeNode *current, cartesianCoords treetopCenter) {
	bool regenerate = false;
	int attempts = 0;
	int maxattempts = 5;
	do{
		attempts++; //pocet pokusu vygenerovani parametru
		if(current->type == TRUNK) { //kmen, nechame ho vest temer rovne
			current->param.relativeVector.theta = exponentialRandom(M_PI*(2.0/180.0)); //nejaky maly odklon, exponencialni rozlozeni se stredem viz druhy parametr
			current->param.relativeVector.phi = uniformRandom(0.0, M_PI*2.0); //rotace - 0-360°
		}else{ //ostatni typy nechame trcet nahodnymi smery
			switch(current->type){
				case BRANCH_UP:
					current->param.relativeVector.theta = normalRandom(M_PI*(35.0/180.0), M_PI*(15.0/180.0)); //nejaky vetsi odklon, normalni rozlozeni se stredem 70° a odchylkou 15°
					break;
				case BRANCH:
					current->param.relativeVector.theta = normalRandom(M_PI*(90.0/180.0), M_PI*(15.0/180.0)); //nejaky vetsi odklon, normalni rozlozeni se stredem 70° a odchylkou 15°
					break;
				case BRANCH_DOWN:
					current->param.relativeVector.theta = normalRandom(M_PI*(130.0/180.0), M_PI*(15.0/180.0)); //nejaky vetsi odklon, normalni rozlozeni se stredem 70° a odchylkou 15°
					break;
				default:
					current->param.relativeVector.theta = normalRandom(M_PI*(70.0/180.0), M_PI*(15.0/180.0)); //nejaky vetsi odklon, normalni rozlozeni se stredem 70° a odchylkou 15°
					break;

			}
			current->param.relativeVector.phi = uniformRandom(0.0, M_PI*2.0); //rotace - 0-360°
		}
		
		fillAbsoluteVector(current); //pocita se s tim, ze delka vetve uz je nastavena

		if(current->param.absoluteVector.theta >= M_PI*(90.0/180.0) && current->param.absoluteVector.theta <= M_PI*(270.0/180.0)){ //veteve vede smerem dolu, nechame pregenerovat
			if(uniformRandom(0.0, 1.0) >= 0.3){ //se 70% pravdepodobnosti nechame takovou vetev pregenerovat
				//pokud nechceme vetve dolu, odkomentovat nasledujici radek, pokud ano, zakomentovat
				regenerate = true;
			}
		}
		if(current->parentNode != NULL){ //pokud to neni rodicovsky prvek, zkontrolujeme, jestli nemiri smerem do koruny
			if(current->param.branchEnd.distanceFrom(treetopCenter) < current->parentNode->param.branchEnd.distanceFrom(treetopCenter)){ //pokud je vzdalenost konce vetve od stredu koruny mensi nez vzdalenost zacatku vetve od stredu koruny, pak vetev miri dovnitr koruny
				if(uniformRandom(0.0, 1.0) >= 0.3){ //se 70% pravdepodobnosti nechame takovou vetev pregenerovat
					regenerate = true;
				}
			}
		}
	}while(regenerate == true && attempts < maxattempts);
}

//smrk
void branchLengthPicea(TreeNode *current, int maxlevel) {
	//delka zavisi na levelu a typu
	float coef;
	coef = (1.0+maxlevel)/current->param.level;
	coef = log10(1.0+100*coef)/2.0; //zavislost pomeru delky vetve na aktualnim levelu
	switch(current->type){
		case TRUNK:
		case TRUNK_BRANCHLESS:
			if(current->parentNode == NULL){ //prvni kmen
				coef *= 5.0;
			}
			current->param.relativeVector.r = 0.02+0.06*uniformRandom(0.7, 1.3)*coef;
			break;
		case BRANCH:
			current->param.relativeVector.r = 0.02+0.02*uniformRandom(0.7, 1.3)*coef;
			break;
		default:
			current->param.relativeVector.r = 0.14*uniformRandom(0.7, 1.3)*coef;
			break;
	}
}
void branchThicknessPicea(TreeNode *current, int maxlevel) {
	current->param.thickness = sqrt(current->param.childLeafs+1.0)*0.0015; //tloustka vetve odpovida poctu vetvi, ktere z teto rostou (musi sedet plocha prurezu)
}

void branchDirectionPicea(TreeNode *current, cartesianCoords treetopCenter) {
	if(current->type == TRUNK || current->type == TRUNK_BRANCHLESS) { //kmen, nechame ho vest temer rovne
		current->param.relativeVector.theta = exponentialRandom(M_PI*(0.5/180.0)); //nejaky maly odklon, exponencialni rozlozeni se stredem viz druhy parametr
		current->param.relativeVector.phi = uniformRandom(0.0, M_PI*2.0); //rotace - 0-360°
	}else if(current->type == BRANCH && current->parentNode != NULL && current->parentNode->type == TRUNK){ //vetev z kmene, nechame ji vest vicemene vodorovne nahodnym smerem z kmene
		current->param.relativeVector.theta = normalRandom(M_PI*(100.0/180.0), M_PI*(5.0/180.0));
		current->param.relativeVector.phi = uniformRandom(0.0, M_PI*2.0); //rotace - 0-360°
	}else if(current->type == BRANCH){ //pokracovani zakladni vetve, nechame ji rust mirne do vyse
		current->param.relativeVector.theta = uniformRandom(M_PI*(1.0/180.0), M_PI*(2.0/180.0)); //nejaky maly odklon
		current->param.relativeVector.phi = normalRandom(M_PI, M_PI*(10.0/180.0)); //rotace kolem 180°
	}else{ //vetev smerujici k zemi
		current->param.relativeVector.theta = normalRandom(M_PI*(90.0/180.0), M_PI*(15.0/180.0)); //odklon kolem 90°
		current->param.relativeVector.phi = normalRandom(M_PI*(0.0/180.0), M_PI*(15.0/180.0)); //rotace kolem 90°
	}

	fillAbsoluteVector(current); //pocita se s tim, ze delka vetve uz je nastavena
}

void fillAbsoluteVector(TreeNode *current) {
	//nejdriv si prevedeme relativni na kartezske

	double rx = current->param.relativeVector.r*sin(current->param.relativeVector.theta)*cos(current->param.relativeVector.phi);
	double ry = current->param.relativeVector.r*sin(current->param.relativeVector.theta)*sin(current->param.relativeVector.phi);
	double rz = current->param.relativeVector.r*cos(current->param.relativeVector.theta);

	if(current->parentNode != NULL){ //pokud mame rodice, je dany vektor relativni k rodicovskemu, tj je nutno prepocitat
		//provedeme rotaci podle absolutniho vektoru rodice (pokud nejaky je) - tim si upravime souradny system tak, aby osa Z smerovala stejne, jako dany vektor
		//1. rotace kolem Y dle rodice
		double r2x = rx*cos(current->parentNode->param.absoluteVector.theta) + rz*sin(current->parentNode->param.absoluteVector.theta);
		double r2y = ry;
		double r2z = -rx*sin(current->parentNode->param.absoluteVector.theta) + rz*cos(current->parentNode->param.absoluteVector.theta);

		//2. rotace kolem Z dle rodice
		double r3x = r2x*cos(current->parentNode->param.absoluteVector.phi) - r2y*sin(current->parentNode->param.absoluteVector.phi);
		double r3y = r2x*sin(current->parentNode->param.absoluteVector.phi) + r2y*cos(current->parentNode->param.absoluteVector.phi);
		double r3z = r2z;

		//3. prevod na sfericke souradnice
		current->param.absoluteVector.r = sqrt(r3x*r3x+r3y*r3y+r3z*r3z);
		current->param.absoluteVector.theta = acos(r3z/sqrt(r3x*r3x+r3y*r3y+r3z*r3z));
		current->param.absoluteVector.phi = atan2(r3y, r3x);

		//dosazeni koncovych souradnic
		current->param.branchEnd.x = current->parentNode->param.branchEnd.x+r3x;
		current->param.branchEnd.y = current->parentNode->param.branchEnd.y+r3y;
		current->param.branchEnd.z = current->parentNode->param.branchEnd.z+r3z;
	}else{ //jinak je uvidni smer [1,0,0], takze je vypocet vyrazne jednodussi
		current->param.absoluteVector.r = current->param.relativeVector.r;
		current->param.absoluteVector.theta = current->param.relativeVector.theta;
		current->param.absoluteVector.phi = current->param.relativeVector.phi;

		current->param.branchEnd.x = rx;
		current->param.branchEnd.y = ry;
		current->param.branchEnd.z = rz;
	}
}

//rand <0, 1) (predpoklada inicializovany srand)
double basicRandom(){
	return ((double) rand() / ((double) RAND_MAX + 1.0));
}

//exponencialni rozdeleni
//mv = mean value
double exponentialRandom(double mv){
	return - mv*log(basicRandom());
}

//normalni rozdeleni
//mean = mean value, var = variability
double normalRandom(double mean, double var){
	int i;
	double sum = 0.0;
	for (i=0; i<12; i++)  sum += basicRandom();

	return (sum-6.0)*var + mean;
}

//uniformni rozdeleni
//l < h
double uniformRandom(double l, double h){
	if(l >= h) return basicRandom();
	return (l + (h - l)*basicRandom());
}

boundingBox combineBoundingBoxes(boundingBox bb1, boundingBox bb2){
	boundingBox combinedBB;

	combinedBB.maxX = max(bb1.maxX, bb2.maxX);
	combinedBB.maxY = max(bb1.maxY, bb2.maxY);
	combinedBB.maxZ = max(bb1.maxZ, bb2.maxZ);

	combinedBB.minX = min(bb1.minX, bb2.minX);
	combinedBB.minY = min(bb1.minY, bb2.minY);
	combinedBB.minZ = min(bb1.minZ, bb2.minZ);

	return combinedBB;
}

boundingBox getBoundingBox(TreeNode *node){
	boundingBox startBB, endBB, combinedBB;
	endBB.minX = endBB.maxX = node->param.branchEnd.x;
	endBB.minY = endBB.maxY = node->param.branchEnd.y;
	endBB.minZ = endBB.maxZ = node->param.branchEnd.z;

	if(node->parentNode == NULL){ //nejvrchnejsi node, druhe souradnice bounding boxu budou nuly
		startBB.maxX = startBB.minX = 0;
		startBB.maxY = startBB.minY = 0;
		startBB.maxZ = startBB.minZ = 0;
	}else{
		startBB.minX = startBB.maxX = node->parentNode->param.branchEnd.x;
		startBB.minY = startBB.maxY = node->parentNode->param.branchEnd.y;
		startBB.minZ = startBB.maxZ = node->parentNode->param.branchEnd.z;
	}
	combinedBB = combineBoundingBoxes(startBB, endBB);
	for(unsigned int i=0;i<node->childNodes.size();i++) {
		combinedBB = combineBoundingBoxes(combinedBB, getBoundingBox(node->childNodes[i]));
	}
	return combinedBB;
}

int maxLevel(TreeNode *node){
	int maxlvl = 1;
	for(unsigned int i=0;i<node->childNodes.size();i++) {
		maxlvl = max(maxlvl, maxLevel(node->childNodes[i])+1);
	}
	return maxlvl;
}

int calcChildLeafs(TreeNode *node){
	int childLeafs = 0;
	for(unsigned int i=0;i<node->childNodes.size();i++) {
		childLeafs += calcChildLeafs(node->childNodes[i]); //vratime soucet ve vsech potomcich
	}
	node->param.childLeafs = childLeafs;
	if(node->childNodes.size() == 0){ //sami jsme listem, vratime 1
		return 1;
	}else{
		return childLeafs;
	}
}
