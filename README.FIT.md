GenTreeBillboardTexture
=======================
Preklad a spusteni ukazkoveho dema na pocitacich v CVT:

Jednoduche demo
----------------
Prikazem `make` se prelozi jednoduche demo, ktere spustite z prikazove radky `./demo`. Pokud si prejete parametrizovat generovani stromu pouzijte program `./demo_gui`, ktery zkompilujete prikazem `make gui`.

POZOR: Na pocitacich v CVT neni nainstalovana podpora [GLADE](http://glade.gnome.org), ktery pouzivame pri vytvareni GUI. Preklad funguje na serveru eva.fit.vutbr.cz prikazem `gmake gui`.

Billboard demo
---------------
3D demo zkompilujete zadanim `make` v adresari `billboard`. Po spusteni souboru `./billboard` se vygeneruje scena obsahujici 30 stromu ruzneho typu chovajici se jako billboardy. Ve scene se muzete pohybovat pomoci klaves `W`, `A`, `S`, `D` a mysi.
