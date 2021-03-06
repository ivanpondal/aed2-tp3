#ifndef ARBOL_BINARIO_H_
#define ARBOL_BINARIO_H_

#include <ostream>
#include <iostream>
#include "Pila.h"
#include "aed2/Lista.h"

using namespace aed2;
using namespace std;

template <typename T>
class ArbolBinario{
	public:
		ArbolBinario();

		~ArbolBinario();

		ArbolBinario(const ArbolBinario&);

		ArbolBinario(const ArbolBinario&, const T&, const ArbolBinario&);

		bool EsNil() const;

		ArbolBinario<T>& Izq();
		ArbolBinario<T>& Der();
		T& Raiz();

		ArbolBinario<T>*& IzqRapido();
		ArbolBinario<T>*& DerRapido();

		const ArbolBinario<T>& Izq() const;
		const ArbolBinario<T>& Der() const;
		const T& Raiz() const;

		Lista<T> Inorder() const;

		ArbolBinario<T>& operator = (const ArbolBinario<T>&);

		bool operator == (const ArbolBinario<T>&) const;

	private:
		struct NodoAb{
			NodoAb(const ArbolBinario<T>& i, const T& r, const ArbolBinario<T>& d) : raiz(T(r)){
				hijos[0] = new ArbolBinario<T>(i);
				hijos[1] = new ArbolBinario<T>(d);
			};

			~NodoAb(){
				delete hijos[0];
				delete hijos[1];
			}

			T raiz;
			ArbolBinario* hijos[2];
		};

		NodoAb* nodo;
};

template <typename T>
ArbolBinario<T>::ArbolBinario(){
	nodo = NULL;
}

template <typename T>
ArbolBinario<T>::~ArbolBinario(){
	delete nodo;
}

template <typename T>
ArbolBinario<T>::ArbolBinario(const ArbolBinario<T>& otro){
	nodo = NULL;
	*this = otro;
}

template <typename T>
ArbolBinario<T>& ArbolBinario<T>::operator = (const ArbolBinario<T>& otro){
	if(this == &otro){
		return *this;
	}

	delete nodo;
	nodo = NULL;

	if(!otro.EsNil()){
		nodo = new NodoAb(otro.Izq(), otro.Raiz(), otro.Der());
	}

	return *this;
}

template <typename T>
ArbolBinario<T>::ArbolBinario(const ArbolBinario<T>& izq, const T& raiz, const ArbolBinario<T>& der){
	nodo = new NodoAb(izq, raiz, der);
}

template <typename T>
bool ArbolBinario<T>::EsNil() const{
	return nodo == NULL;
}

template <typename T>
ArbolBinario<T>& ArbolBinario<T>::Izq(){
	#ifdef DEBUG
		assert(nodo != NULL);
	#endif
	return *nodo->hijos[0];
}

template <typename T>
const ArbolBinario<T>& ArbolBinario<T>::Izq() const{
	#ifdef DEBUG
		assert(nodo != NULL);
	#endif
	return *nodo->hijos[0];
}

template <typename T>
ArbolBinario<T>*& ArbolBinario<T>::IzqRapido(){
	#ifdef DEBUG
		assert(nodo != NULL);
	#endif
	return nodo->hijos[0];
}

template <typename T>
ArbolBinario<T>& ArbolBinario<T>::Der(){
	#ifdef DEBUG
		assert(nodo != NULL);
	#endif
	return *nodo->hijos[1];
}

template <typename T>
const ArbolBinario<T>& ArbolBinario<T>::Der() const{
	#ifdef DEBUG
		assert(nodo != NULL);
	#endif
	return *nodo->hijos[1];
}

template <typename T>
ArbolBinario<T>*& ArbolBinario<T>::DerRapido(){
	#ifdef DEBUG
		assert(nodo != NULL);
	#endif
	return nodo->hijos[1];
}

template <typename T>
T& ArbolBinario<T>::Raiz(){
	#ifdef DEBUG
		assert(nodo != NULL);
	#endif
	return nodo->raiz;
}

template <typename T>
const T& ArbolBinario<T>::Raiz() const{
	#ifdef DEBUG
		assert(nodo != NULL);
	#endif
	return nodo->raiz;
}

template <typename T>
Lista<T> ArbolBinario<T>::Inorder() const{
	Lista<T> listaAbInorder;

	const ArbolBinario<T>* ptrAbIt = this;
	Pila<const ArbolBinario<T>*> pilaPtrAb;
	bool done = false;

	while(!done){
		if(!ptrAbIt->EsNil()){
			pilaPtrAb.Apilar(ptrAbIt);
			ptrAbIt = &ptrAbIt->Izq();
		}
		else{
			if(!pilaPtrAb.EsVacia()){
				listaAbInorder.AgregarAtras(pilaPtrAb.Tope()->Raiz());
				ptrAbIt = &pilaPtrAb.Tope()->Der();
				pilaPtrAb.Desapilar();
			}
			else{
				done = true;
			}
		}
	}

	return listaAbInorder;
}

template <typename T>
bool ArbolBinario<T>::operator == (const ArbolBinario<T>& otro) const{
	bool sonIguales = false;
	sonIguales = (EsNil() == otro.EsNil());

	if(sonIguales && !EsNil()){
		sonIguales = (Raiz() == otro.Raiz()) && (Der() == otro.Der()) && (Izq() == otro.Izq());
	}

	return sonIguales;
}
#endif // ARBOL_BINARIO_H_INCLUDED
