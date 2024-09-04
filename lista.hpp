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

	//lisätään Listan alkuun alkio (onnistuu aina)
	void addStart(void *dataptr);
	//lisätään Listan loppuun alkio (onnistuu aina)
	void add(void *dataptr);
	//poistetaan Listan viimeinen alkio (palauttaa false jos Lista on tyhjä)
	bool removeLast();
	//lisätään työpointterin jälkeen alkio (palauttaa false jos työpointteri ei ole kelvollinen)
	bool insertBefore(void *dataprt);
	//lisätään työpointterin jälkeen alkio (palauttaa false jos työpointteri ei ole kelvollinen)
	bool insert(void *dataptr);
	//poistetaan alkio työpointterin kohdalta
	bool remove();

	// poistaa nykyisen alkion, mutta ei tyhjennä payloadin muistia
	bool removeSavePayload();

	//tuhotaan koko Lista ja vapautetaan kaikki tiedot
	void destroy();
	//tuhotaan koko Lista mutta säilytetään ndataptr sisältö
	void destroySavePayload();
	//poistetaan Listan viimeinen alkio (palauttaa false jos Lista on tyhjä) [säilytetään payload]
	bool removeLastSavePayload();

	//asettaa työpointterin Listan alkuun (palauttaa false jos Lista on tyhjä)
	bool goToStart();
	//asettaa työpointterin Listan loppuun (palauttaa false jos Lista on tyhjä)
	bool goToEnd();
	//liikuttaa työpointteria eteenpäin (palauttaa false jos Lista on loppu)
	bool goForward();
	//liikuttaa työpointteria taaksepäin (palauttaa false jos ollaan alussa)
	bool goBackwards();
	//menee haluttuun nodeen (palauttaa false jos ei onnistu)
	bool goToNode(int number); 

	//tutkii, ollaanko ekassa nodessa
	bool isAtEnd();
	//tutkii, ollaanko vikassa nodessa
	bool isAtStart();

	//antaa ensimmäisen noden osoittaman tiedon
	void *getFirst();
	//antaa viimeisen noden osoittaman tiedon
	void *getLast();
	//antaa työpointterin osoittaman tiedon
	void *getCurrent();
	//asettaa työpointterin nodeen uuden tiedon (tuhoaa vanhan jos siellä sellaista on)
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

