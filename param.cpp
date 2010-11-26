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
	if(current->parentNode == NULL){
		current->param.thickness = 1/8.0; //zakladni tloustka kmene
	}else if(current->parentNode->type == TRUNK && current->type != TRUNK){ //vetev trcici z kmene, bude tensi
		current->param.thickness = current->parentNode->param.thickness*0.6;
	}else if(current->type == TRUNK){ //kmen, nebudeme tolik ztencovat
		current->param.thickness = current->parentNode->param.thickness*0.85;
	}else{
		current->param.thickness = current->parentNode->param.thickness*0.7;
	}
	//current->param.thickness = (1.0/(current->param.level+1.0))/7.0; //v pomeru k delce je vetev 7x uzsi (pri zachovani vypoctu)
}

void branchDirectionTree1(TreeNode *current) {
	bool regenerate = false;
	int attempts = 0;
	int maxattempts = 5;
	do{
		attempts++; //pocet pokusu vygenerovani parametru
		if(current->type == TRUNK) { //kmen, nechame ho vest temer rovne
			current->param.relativeVector.theta = exponentialRandom(M_PI*(2.0/180.0)); //nejaky maly odklon, exponencialni rozlozeni se stredem 5°
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
		//delku vetve spocitame z levelu
		fillAbsoluteVector(current); //pocita se s tim, ze delka vetve uz je nastavena
		if(current->param.absoluteVector.theta >= M_PI*(90.0/180.0) && current->param.absoluteVector.theta <= M_PI*(270.0/180.0)){ //veteve vede smerem dolu, nechame pregenerovat
			//pokud nechceme vetve dolu, odkomentovat nasledujici radek
			//regenerate = true;
		}
	}while(regenerate == true && attempts < maxattempts);
}

void fillAbsoluteVector(TreeNode *current) {
	//nejdriv si prevedeme relativni na kartezske
/*
double ax = 0.0, ay = 0.0, az = 1.0;
double r = 1.0;
double phi = 1.2;
double theta = 1.0;
double bx = r*sin(theta)*cos(phi);
double by = r*sin(theta)*sin(phi);
double bz = r*cos(theta);

double cx = ax*cos(theta) + az*sin(theta);
double cy = ay;
double cz = -ax*sin(theta) + az*cos(theta);

double dx = cx*cos(phi) - cy*sin(phi);
double dy = cx*sin(phi) + cy*cos(phi);
double dz = cz;
cout << "[" << ax << ", " << ay << ", " << az << "]";
cout << "[" << bx << ", " << by << ", " << bz << "]";
cout << "[" << cx << ", " << cy << ", " << cz << "]";
cout << "[" << dx << ", " << dy << ", " << dz << "]";
exit(1);
*/

	double rx = current->param.relativeVector.r*sin(current->param.relativeVector.theta)*cos(current->param.relativeVector.phi);
	double ry = current->param.relativeVector.r*sin(current->param.relativeVector.theta)*sin(current->param.relativeVector.phi);
	double rz = current->param.relativeVector.r*cos(current->param.relativeVector.theta);

	if(current->parentNode != NULL){ //pokud mame rodice, je dany vektor relativni k rodicovskemu, tj je nutno prepocitat
		//provedeme rotaci podle absolutniho vektoru rodice (pokud nejaky je) - tim si upravime souradny system tak, aby osa Y smerovala stejne, jako dany vektor
/*
		//nejprve rotace kolem osy Y
		double tyx = rx*cos(current->parentNode->param.absoluteVector.theta) + rz*sin(current->parentNode->param.absoluteVector.theta);
		double tyy = ry;
		double tyz = -rx*sin(current->parentNode->param.absoluteVector.theta) + rz*cos(current->parentNode->param.absoluteVector.theta);

		//potom rotace kolem osy Z
		double tzx = tyx*cos(current->parentNode->param.absoluteVector.phi) - tyy*sin(current->parentNode->param.absoluteVector.phi);
		double tzy = tyx*sin(current->parentNode->param.absoluteVector.phi) + tyy*cos(current->parentNode->param.absoluteVector.phi);
		double tzz = tyz;

		double finx = tzx;
		double finy = tzy;
		double finz = tzz;

		//mame vysledne kartezske souradnice relativni ke smeru predchozi vetve
		//spocitame jeste absolutni vektor

		current->param.absoluteVector.r = sqrt(finx*finx+finy*finy+finz*finz);
		current->param.absoluteVector.theta = acos(finz/sqrt(finx*finx+finy*finy+finz*finz));
		current->param.absoluteVector.phi = atan2(finy, finx);
*/
//TODO: toto neni dobre, chce to otocit podle otocenych os
/*
		current->param.absoluteVector.r = current->param.relativeVector.r;
		current->param.absoluteVector.phi = current->parentNode->param.absoluteVector.phi+current->param.relativeVector.phi;
		current->param.absoluteVector.theta = current->parentNode->param.absoluteVector.theta+current->param.relativeVector.theta;

		//trosku to znormalizujeme, at nemame zbytecne uhly -4PI napr.
		if(current->param.absoluteVector.phi >= M_PI*2.0){
			current->param.absoluteVector.phi = fmod(current->param.absoluteVector.phi, float(M_PI*2.0));
		}else if(current->param.absoluteVector.phi <= 0.0){
			current->param.absoluteVector.phi = fmod(current->param.absoluteVector.phi, float(M_PI*2.0))+M_PI*2.0;
		}
		if(current->param.absoluteVector.theta >= M_PI*2.0){
			current->param.absoluteVector.theta = fmod(current->param.absoluteVector.theta, float(M_PI*2.0));
		}else if(current->param.absoluteVector.theta <= 0.0){
			current->param.absoluteVector.theta = fmod(current->param.absoluteVector.theta, float(M_PI*2.0))+M_PI*2.0;
		}
		rx = current->param.absoluteVector.r*sin(current->param.absoluteVector.theta)*cos(current->param.absoluteVector.phi);
		ry = current->param.absoluteVector.r*sin(current->param.absoluteVector.theta)*sin(current->param.absoluteVector.phi);
		rz = current->param.absoluteVector.r*cos(current->param.absoluteVector.theta);

		current->param.branchEnd.x = current->parentNode->param.branchEnd.x+rx;
		current->param.branchEnd.y = current->parentNode->param.branchEnd.y+ry;
		current->param.branchEnd.z = current->parentNode->param.branchEnd.z+rz;
*/
		//TODO: porad to patrne neni uplne spravne, zkontrolovat prevod zpet na sfericke
		//vychozi souradnice
		double r0x = 0.0;
		double r0y = 0.0;
		double r0z = current->param.relativeVector.r;
		//1. rotace kolem Y rodice, ktery je otoceny o Z
		//1. a) rotace kolem aktualni Y
		double r1x = r0x*cos(current->param.relativeVector.theta) + r0z*sin(current->param.relativeVector.theta);
		double r1y = r0y;
		double r1z = -r0x*sin(current->param.relativeVector.theta) + r0z*cos(current->param.relativeVector.theta);

		//1. b)rotace kolem osy Z rodice
		//double r2x = r1x*cos(current->parentNode->param.relativeVector.phi) - r1y*sin(current->parentNode->param.relativeVector.phi);
		//double r2y = r1x*sin(current->parentNode->param.relativeVector.phi) + r1y*cos(current->parentNode->param.relativeVector.phi);
		//double r2z = r1z;

		//2. rotace kolem rodice
		//2. a) rotace kolem Z dle rodice, inverzni, tedy anuluje 1. b)
		//2. b) rotace kolem Y rodice, inverzni
		double r2bx = r1x*cos(-current->parentNode->param.relativeVector.theta) + r1z*sin(-current->parentNode->param.relativeVector.theta);
		double r2by = r1y;
		double r2bz = -r1x*sin(-current->parentNode->param.relativeVector.theta) + r1z*cos(-current->parentNode->param.relativeVector.theta);

		//2. c) rotace kolem Z
		double r2cx = r2bx*cos(current->param.relativeVector.phi) - r2by*sin(current->param.relativeVector.phi);
		double r2cy = r2bx*sin(current->param.relativeVector.phi) + r2by*cos(current->param.relativeVector.phi);
		double r2cz = r2bz;

		//2. d) rotace kolem Y rodice, inverzni
		double r2dx = r2cx*cos(current->parentNode->param.relativeVector.theta) + r2cz*sin(current->parentNode->param.relativeVector.theta);
		double r2dy = r2cy;
		double r2dz = -r2cx*sin(current->parentNode->param.relativeVector.theta) + r2cz*cos(current->parentNode->param.relativeVector.theta);

		//2. c) rotace kolem Z rodice
		double r2ex = r2dx*cos(current->parentNode->param.relativeVector.phi) - r2dy*sin(current->parentNode->param.relativeVector.phi);
		double r2ey = r2dx*sin(current->parentNode->param.relativeVector.phi) + r2dy*cos(current->parentNode->param.relativeVector.phi);
		double r2ez = r2dz;

		//3. prevod na sfericke souradnice
		double finx = r2ex;
		double finy = r2ey;
		double finz = r2ez;
		current->param.absoluteVector.r = sqrt(finx*finx+finy*finy+finz*finz);
		current->param.absoluteVector.theta = acos(finz/sqrt(finx*finx+finy*finy+finz*finz));
		current->param.absoluteVector.phi = atan2(finy, finx);

		//dosazeni koncovych souradnic
		current->param.branchEnd.x = current->parentNode->param.branchEnd.x+finx;
		current->param.branchEnd.y = current->parentNode->param.branchEnd.y+finy;
		current->param.branchEnd.z = current->parentNode->param.branchEnd.z+finz;
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
