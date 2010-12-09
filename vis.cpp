#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "vis.h"

/** Makro pro rychlé prohození dvou proměnných. */
#define SWAP(a, b) a ^= b; b ^= a; a ^= b;

/** Pomocná funkce na omezení hodonoty celočiselné proměnné.
 * @param val Hodnota pro omezení
 * @param min Nejmenší přípustná hodnota
 * @param max Největší přípustná hodnota
 */
int clamp(int val, int min, int max) {
	if (val < min) {
		return min;
	} else if (val > max) {
		return max;
	}
	return val;
}

/** Pomocná funkce na omezení hodonoty proměnné v plovoucí desetinné čárce.
 * @param val Hodnota pro omezení
 * @param min Nejmenší přípustná hodnota
 * @param max Největší přípustná hodnota
 */
float fclamp(float val, float min, float max) {
	if (val < min) {
		return min;
	} else if (val > max) {
		return max;
	}
	return val;
}

/** Kreslení pixelu.
 * Nakreslí pixel zadané barvy a přidá šum.
 * @param surface Ukazatel na SDL Surface
 * @param x X (vodorovná) souřadnice pixelu
 * @param y Y (svislá) souřadnice pixelu
 * @param c Barva pixelu
 * @param alpha Průhlednost pixelu
 */
void putpixel(SDL_Surface *surface, size_t x, size_t y, SDL_Color c, unsigned int alpha = 255) {
	// Presahuje rozmery surface, zapis do neplatne pameti
	if (x >= abs(surface->w) || y >= abs(surface->h)) {
		return;
	}
	const float noise = 0.8;
	const double random = 1 + basicRandom() * noise - noise / 2;
	c.r = clamp(c.r * random, 0, 255);
	c.g = clamp(c.g * random, 0, 255);
	c.b = clamp(c.b * random, 0, 255);
	
	//pouzivame napevno 32bitovou barevnou hloubku
	Uint32* bufp;
	bufp = (Uint32*) surface->pixels + y  *surface->pitch / 4 + x;
	//spocitame novou barvu pomoci apha scitani barev (zdrojova barva ma vzdy nulovou pruhlednost, tj. u ni alphu ignorujeme)
	Uint8 orig_r;
	Uint8 orig_g;
	Uint8 orig_b;
	Uint8 orig_a;
	SDL_GetRGBA(*bufp, surface->format, &orig_r, &orig_g, &orig_b, &orig_a);
	float a_o = orig_a / 255.0;
	float a_n = alpha / 255.0;
	Uint8 new_r = c.r * a_n + orig_r * a_o * (1 - a_n);
	Uint8 new_g = c.g * a_n + orig_g * a_o * (1 - a_n);
	Uint8 new_b = c.b * a_n + orig_b * a_o * (1 - a_n);
	Uint8 new_a = (a_n + a_o * (1 - a_n)) * 255;
	
	Uint32 color = SDL_MapRGBA(surface->format, new_r, new_g, new_b, new_a);
	*bufp = color;
}

/** Kreslení listu.
 * Nakreslí list na zadanou souřadnici.
 * @param canvas Ukazatel na SDL Surface
 * @param x X (vodorovná) souřadnice listu
 * @param y Y (svislá) souřadnice listu
 * @param color Barva pixelu
 * @param size_ex Velikost listu
 */
void drawleaf(SDL_Surface *canvas, size_t x, size_t y, SDL_Color color, float size_ex) {
	//std::clog << size_ex << '\n';
	unsigned int size = size_ex;
	
	if(size < 1){
		size = 1;
	}
	
	x -= size / 2;
	y -= size / 2;
	
	float alpha;
	float alpha1, alpha2;
	float tx, ty;
	for (unsigned int i = 0; i <= size / 2 + 1; i++) {	//list je ctverec, u stredu zcela nepruhledny, cim vice od stredu, tim pruhlednejsi
		for (unsigned int j = 0; j <= size / 2 + 1; j++) {
			tx = j;
			ty = i;
			alpha1 = fclamp(1 - abs(tx) / size_ex * 2, 0, 1) * 2;
			alpha1 += fclamp(1 - abs(tx - 0.5) / size_ex * 2, 0, 1);
			alpha1 += fclamp(1 - abs(tx + 0.5) / size_ex * 2, 0, 1);
			alpha1 /= 4;
			alpha2 = fclamp(1 - abs(ty) / size_ex * 2, 0, 1) * 2;
			alpha2 += fclamp(1 - abs(ty + 0.5) / size_ex * 2, 0, 1);
			alpha2 += fclamp(1 - abs(ty - 0.5) / size_ex * 2, 0, 1);
			alpha2 /= 4;
			alpha = alpha1 * alpha2;
			
			if (alpha > 0) {	//jenom pokud ma smysl kreslit
				putpixel(canvas, x + tx, y + ty, color, alpha * 255);
				if (tx > 0) {	//pouze, pokud jsem tento pixel jiz nekreslili (na stredu listu se tato hodnota prekryva)
					putpixel(canvas, x - tx, y + ty, color, alpha * 255);
				}
				if(ty > 0) {
					putpixel(canvas, x + tx, y - ty, color, alpha * 255);
				}
				if(tx > 0 && ty > 0) {
					putpixel(canvas, x - tx, y - ty, color, alpha * 255);
				}
			}
		}
	}
}

/** Kreslení větve.
 * Kreslí větev s listy na zadané souřadnice. Nejdřív se kreslí větev, poté se kreslí listy. Pro kreslení se používá Bresenhamův algoritmus pro kreslení úsečky.
 * @param canvas Ukazatel na SDL Surface
 * @param x1 X (vodorovná) souřadnice počátku větve
 * @param y1 Y (svislá) souřadnice počátku větve
 * @param x2 X (vodorovná) souřadnice konce větve
 * @param y2 Y (svislá) souřadnice konce větve
 * @param thickness Tloušťka větve
 * @param wood Barva dřeva
 * @param leaf Barva listů
 * @param leafinterval Interval místění listů na větev
 * @param leafsize Velikost listů
 */
void drawbranch(SDL_Surface *canvas, int x1, int y1, int x2, int y2, float thickness, SDL_Color wood, SDL_Color leaf, float leafinterval, float leafsize) {
	//std::clog << "Line: " << '[' << x1 << ", " << y1 << "] -> [" << x2 << ", " << y2 << ']' << '\n';
	//std::clog << leafinterval << '\n';
	
	x1 = clamp(x1, 0, canvas->w - 1);
	y1 = clamp(y1, 0, canvas->h - 1);
	x2 = clamp(x2, 0, canvas->w - 1);
	y2 = clamp(y2, 0, canvas->h - 1);
	//std::clog << "Clamp: " << '[' << x1 << ", " << y1 << "] -> [" << x2 << ", " << y2 << ']' << '\n';
	float thickness_orig = thickness;
	if (thickness < 1) {
		thickness = 1;
	}
	if (leafinterval == 0 || leafinterval > max(canvas->w, canvas->h)) {
		leafinterval = max(canvas->w, canvas->h);
	}
	size_t leafinterval_cut = leafinterval;
	
	const bool steep = abs(y1 - y2) > abs(x1 - x2);
	if (steep) {
		SWAP(x1, y1);
		SWAP(x2, y2);
	}
	if (x1 > x2) {
		SWAP(x1, x2);
		SWAP(y1, y2);
	}
	short step_y = 1;
	if (y1 > y2) {
		step_y = -1;
	}
	
	const unsigned int dx = abs(x2 - x1);
	const unsigned int dy = abs(y2 - y1);
	int P = 2 * dy - dx;
	const unsigned int P1 = 2 * dy;
	const int P2 = P1 - 2 * dx;
	unsigned int y = y1;
	SDL_Color wood_orig = wood;
	
	for (int x = x1; x <= x2; x++) {	//Vetev
		for (size_t t = thickness; t > 0; t--) {
			wood = wood_orig;
			float koef = sqrt((thickness-abs(thickness-(t*2.0-0.5)))/(thickness)); //pixel, vycentrovany, z nej spocitana tmavost kmene
			koef += sqrt((thickness-abs(thickness-(t*2.0)))/(thickness)); //hodnota na jednom kraji pixelu
			koef += sqrt((thickness-abs(thickness-(t*2.0-1.0)))/(thickness)); //hodnota na druhem kraji pixelu
			koef /= 3.0; //prumer
			wood.r *= koef;
			wood.g *= koef;
			wood.b *= koef;
			float alpha = 255*min(sqrt(sqrt(koef*10)), 1.0f);
			alpha *= float(thickness_orig)/thickness;
			if (steep) {
				putpixel(canvas, y - thickness / 2 + t, x, wood, alpha);
			} else {
				putpixel(canvas, x , y - thickness / 2 + t, wood, alpha);
			}
		}
		
		if (P >= 0) {
			P += P2;
			y += step_y;
		} else {
			P += P1;
		}
	}
	
	y = y1;
	SDL_Color leaf_orig = leaf;
	int noofleafs = 0; //pocet listu, ktere vykreslit
	int lim = max(canvas->w, canvas->h); //hranicni hodnota pro kresleni listu
	for (int x = x1; x <= x2; x++) {	//Listy
		noofleafs = 0;
		if(leafinterval != lim){
			if(leafinterval_cut != 0 && (x % leafinterval_cut) == 0){
				noofleafs = 1; //vykreslime 1 list
			}else if(leafinterval_cut == 0 && leafinterval > 0.0){ //pokud je to po zaokrouhleni nula, ale jinak ne, chceme list s pravdepodobnosti odpovidajici leafintervalu 
				float fnoofleafs = 1.0/leafinterval; //pocet listu k vykresleni je 1/interval mezi nimi, tedy pocet listu na pixel
				noofleafs = fnoofleafs; //zaokrouhlime dolu
				if(basicRandom() >= fnoofleafs-noofleafs){ //pokud je random vetsi, nez desetinna cast poctu listu k vukresleni, pridame 1 list k vykresleni
					noofleafs += 1;
				}
			}
		}
		for(int le=0; le<noofleafs; le++) {
			leaf = leaf_orig;
			float yrand = basicRandom();
			float xrand = basicRandom();
			//size_t yoffset = yrand * thickness - thickness / 2;
			//size_t xoffset = xrand * dx - dx / 2;
			size_t yoffset = yrand * leafsize*2.0 - leafsize / 2.0;
			size_t xoffset = xrand * leafsize*2.0 - leafsize / 2.0;
			float koef = xrand;
			koef = 1.0+koef*1.7-0.5;
			leaf.r = clamp(leaf.r*koef, 0, 255);
			leaf.g = clamp(leaf.g*koef, 0, 255);
			leaf.b = clamp(leaf.b*koef, 0, 255);
			int tx, ty;
			if (steep) {
				tx = y + yoffset;
				ty = x + xoffset;
			} else {
				tx = x + xoffset;
				ty = y + yoffset;
			}
			int ntx = clamp(tx, 0, canvas->w);
			int nty = clamp(ty, 0, canvas->h);
			if(ntx == tx && nty == ty){ //jen pokud lezi bod uvnitr obrazu
				drawleaf(canvas, tx, ty, leaf, leafsize);
			}
		}
		
		if (P >= 0) {
			P += P2;
			y += step_y;
		} else {
			P += P1;
		}
	}
}

/** Vizualizace stromu.
 * Funkce BFS algoritmem prochází strom, a kreslí jeho větve s listy podle zadaných parametrů.
 * @param data Ukazatel na SDLSurface
 * @param tree Struktura s parametry stromu
 * @param wood Barva dřeva
 * @param leafs Barva listů
 */
void GenTreeBillboardTexture_visualize(SDL_Surface * data, TreeNode* tree, SDL_Color wood, SDL_Color leafs) {
	//std::clog << "Visualizing tree...\n";
	
	boundingBox bounds = getBoundingBox(tree);
	cartesianCoords tbs = getAndResetTreeBase(tree);
	//vycentrovani kmene
	float mdiff = max(abs(bounds.minX-tbs.x), abs(bounds.maxX-tbs.x));
	bounds.minX = tbs.x-mdiff;
	bounds.maxX = tbs.x+mdiff;
	//std::clog << "Bounds: " << (std::string)(bounds) << '\n';
	const float treeWidth = abs(bounds.minX - bounds.maxX);
	const float treeHeight = abs(bounds.minZ - bounds.maxZ);
	
	const float treeAspect = treeWidth / treeHeight;
	//std::clog << "Tree aspect ratio: " << treeAspect << '\n';
	const float imageAspect = (float) data->w / (float) data->h;
	//std::clog << "Image aspect ratio: " << imageAspect << '\n';
	float scale = 1;
	size_t offset = 0;
	if (treeAspect < imageAspect) {
		scale = data->h / treeHeight;
		offset = (data->w - treeWidth * scale) / 2;
	} else {
		scale = data->w / treeWidth;
	}
	
	queue<TreeNode*> q;
	q.push(tree);
	SDL_Color wood_orig = wood;
	while (!q.empty()) {
		wood = wood_orig;
		//std::clog << q.size() << '\n';
		TreeNode* node = q.front();
		
		cartesianCoords origin;
		if (!node->parentNode) {
			origin.x = 0;
			origin.z = 0;
		} else {
			origin = node->parentNode->param.branchEnd;
		}
		
		if(node->param.branchvis == LVIS_LEAF){ //pro vetve, ktere se maji vizualizovat barvou listu
			wood = leafs; //pouzijeme i pro drevo barvu listu
		}
		drawbranch(data,
			(origin.x + abs(bounds.minX)) * scale + offset,
			data->h - 1 - (origin.z + abs(bounds.minZ)) * scale,
			(node->param.branchEnd.x + abs(bounds.minX)) * scale + offset,
			data->h - (node->param.branchEnd.z + abs(bounds.minZ)) * scale,
			node->param.thickness * scale,
			wood,
			leafs,
			(node->param.leafsize * scale) * (1 / node->param.leafs),
			node->param.leafsize * scale);
		
		for (size_t i = 0; i < node->childNodes.size(); i++) {
			//std::clog << "Pushing new node\n";
			q.push(node->childNodes[i]);
		}
		
		q.pop();
	}
	
	//std::clog << "Vis done!\n";
	return;
}
