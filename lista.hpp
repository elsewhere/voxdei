#ifndef _LISTA_H_
#define _LISTA_H_

class ListanSolmu
{
public:
	//muodostimet ja tuhoajat
	ListanSolmu();
	ListanSolmu(void *ptr);
	~ListanSolmu();

	//asettaa solmun datan halutuksi
	void set(void *ptr);

	//tulostaa tiedot
	void print();

	//osoittimet seuraaviin osiin
	class ListanSolmu *next;
	class ListanSolmu *previous;

	//data, jota solmu kantaa
	void *payload;
};

class Lista
{
public:
	Lista();
	~Lista();

	//lis�t��n Listan alkuun alkio (onnistuu aina)
	void addStart(void *dataptr);
	//lis�t��n Listan loppuun alkio (onnistuu aina)
	void add(void *dataptr);
	//poistetaan Listan viimeinen alkio (palauttaa false jos Lista on tyhj�)
	bool removeLast();
	//lis�t��n ty�pointterin j�lkeen alkio (palauttaa false jos ty�pointteri ei ole kelvollinen)
	bool insertBefore(void *dataprt);
	//lis�t��n ty�pointterin j�lkeen alkio (palauttaa false jos ty�pointteri ei ole kelvollinen)
	bool insert(void *dataptr);
	//poistetaan alkio ty�pointterin kohdalta
	bool remove();

	// poistaa nykyisen alkion, mutta ei tyhjenn� payloadin muistia
	bool removeSavePayload();

	//tuhotaan koko Lista ja vapautetaan kaikki tiedot
	void destroy();
	//tuhotaan koko Lista mutta s�ilytet��n ndataptr sis�lt�
	void destroySavePayload();
	//poistetaan Listan viimeinen alkio (palauttaa false jos Lista on tyhj�) [s�ilytet��n payload]
	bool removeLastSavePayload();

	//asettaa ty�pointterin Listan alkuun (palauttaa false jos Lista on tyhj�)
	bool goToStart();
	//asettaa ty�pointterin Listan loppuun (palauttaa false jos Lista on tyhj�)
	bool goToEnd();
	//liikuttaa ty�pointteria eteenp�in (palauttaa false jos Lista on loppu)
	bool goForward();
	//liikuttaa ty�pointteria taaksep�in (palauttaa false jos ollaan alussa)
	bool goBackwards();
	//menee haluttuun nodeen (palauttaa false jos ei onnistu)
	bool goToNode(int number); 

	//tutkii, ollaanko ekassa nodessa
	bool isAtEnd();
	//tutkii, ollaanko vikassa nodessa
	bool isAtStart();

	//antaa ensimm�isen noden osoittaman tiedon
	void *getFirst();
	//antaa viimeisen noden osoittaman tiedon
	void *getLast();
	//antaa ty�pointterin osoittaman tiedon
	void *getCurrent();
	//asettaa ty�pointterin nodeen uuden tiedon (tuhoaa vanhan jos siell� sellaista on)
	void setCurrent(void *ptr);
	//kertoo kuinka monta solmua Listassa on
	int getNodeCount();
	//kertoo nykyisen noden
	int getCurrentNodeNumber();

	//tulostetaan Lista lokiin (debug)
	void print();

	int solmuja;
	ListanSolmu *start;
	ListanSolmu *current;
	ListanSolmu *end;
};

#endif

