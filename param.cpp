#include <stdio.h>
#include <stdlib.h>
#include "param.h"
#include <cmath>

const treeParams treeBuilders[PTREE_COUNT] = {
   /* PTREE_1 */ {branchLengthTree1, branchThicknessTree1, branchDirectionTree1}
};

void GenTreeBillboardTexture_parametrize(TreeNode *tree, PTreeType treeType, int seed) { //oparamerizuje dany strom
	srand(seed); //zinicializujeme rand
	parametrizeNode(tree, treeType);
	return;
}

void parametrizeNode(TreeNode *node, PTreeType treeType, int level) {
	//nejprve parametrizace daneho uzlu
	node->param.level = level;
	//zavolame prislusne funkce parametrizujici v danem typu stromu
	treeBuilders[treeType].branchLengthFunc(node);
	treeBuilders[treeType].branchThicknessFunc(node);
	treeBuilders[treeType].branchDirectionFunc(node);

	//pote nechame projit vsechny potomky
	for(unsigned int i=0;i<node->childNodes.size();i++) {
		parametrizeNode(node->childNodes[i], treeType, level+1);
	}
}

void branchLengthTree1(TreeNode *current) {
	//delku vetve spocitame z levelu
	current->param.relativeVector.r = 1.0/(current->param.level+1.0);
}
void branchThicknessTree1(TreeNode *current) {
	//tloustku vetve spocitame z levelu
	current->param.thickness = (1.0/(current->param.level+1.0))/7.0; //v pomeru k delce je vetev 7x uzsi (pri zachovani vypoctu)
}

void branchDirectionTree1(TreeNode *current) {
	if(current->type == TRUNK) { //kmen, nechame ho vest temer rovne
		current->param.relativeVector.theta = exponentialRandom(M_PI*(3.0/180.0)); //nejaky maly odklon, exponencialni rozlozeni se stredem 5°
		current->param.relativeVector.phi = uniformRandom(0.0, M_2_PI); //rotace - 0-360°
	}else{ //ostatni typy nechame trcet nahodnymi smery
		current->param.relativeVector.theta = normalRandom(M_PI*(70.0/180.0), M_PI*(15.0/180.0)); //nejaky vetsi odklon, normalni rozlozeni se stredem 70° a odchylkou 15°
		current->param.relativeVector.phi = uniformRandom(0.0, M_2_PI); //rotace - 0-360°
	}
	//delku vetve spocitame z levelu
	fillAbsoluteVector(current); //pocita se s tim, ze delka vetve uz je nastavena
}

void fillAbsoluteVector(TreeNode *current) {
	//nejdriv si prevedeme relativni na kartezske
	double rx = current->param.relativeVector.r*sin(current->param.relativeVector.theta)*cos(current->param.relativeVector.phi);
	double ry = current->param.relativeVector.r*sin(current->param.relativeVector.theta)*sin(current->param.relativeVector.phi);
	double rz = current->param.relativeVector.r*cos(current->param.relativeVector.theta);

	if(current->parentNode != NULL){ //pokud mame rodice, je dany vektor relativni k rodicovskemu, tj je nutno prepocitat
		//provedeme rotaci podle absolutniho vektoru rodice (pokud nejaky je) - tim si upravime souradny system tak, aby osa Y smerovala stejne, jako dany vektor
		//nejprve rotace kolem osy Z
		double tzx = rx*cos(current->parentNode->param.absoluteVector.phi) - ry*sin(current->parentNode->param.absoluteVector.phi);
		double tzy = rx*sin(current->parentNode->param.absoluteVector.phi) + ry*cos(current->parentNode->param.absoluteVector.phi);
		double tzz = rz;

		//potom rotace kolem osy Y
		double tyx = tzx*cos(current->parentNode->param.absoluteVector.theta) - tzz*sin(current->parentNode->param.absoluteVector.theta);
		double tyy = tzy;
		double tyz = tzx*sin(current->parentNode->param.absoluteVector.theta) + tzz*cos(current->parentNode->param.absoluteVector.theta);

		//mame vysledne kartezske souradnice relativni ke smeru predchozi vetve
		//spocitame jeste absolutni vektor

		current->param.absoluteVector.r = sqrt(tyx*tyx+tyy*tyy+tyz*tyz);
		current->param.absoluteVector.theta = acos(tyz/sqrt(tyx*tyx+tyy*tyy+tyz*tyz));
		current->param.absoluteVector.phi = atan2(tyy, tyx);

		current->param.branchEnd.x = current->parentNode->param.branchEnd.x+tyx;
		current->param.branchEnd.y = current->parentNode->param.branchEnd.y+tyy;
		current->param.branchEnd.z = current->parentNode->param.branchEnd.z+tyz;
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
