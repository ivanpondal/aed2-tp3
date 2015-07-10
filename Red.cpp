#include "Red.h"
#include "Pila.h"

using namespace std;
using namespace aed2;

Red::Red () {
	//compus = Conj<Compu>();
	//dns = DiccString<NodoRed>();
}
//Red Red::operator=(const Red& otro) {
	// To Do
//}
Red::Red (const Red& otro) {

	// copia el conjunto de tuplas
	compus = otro.compus;
	// rearma los nodos (con conexiones en blanco) del diccionario dns
	Conj<Compu>::Iterador it = compus.CrearIt();
	while(it.HaySiguiente()) {
		Compu c = it.Siguiente();
		NodoRed nr = otro.dns.Significado(c.ip);
		Dicc<String, Conj<Camino> > auxcaminos = nr.caminos;
		Dicc <Interfaz, NodoRed* > auxConexiones;
		NodoRed auxNr;
		auxNr.pc = c;
		auxNr.caminos = auxcaminos;
		auxNr.conexiones = auxConexiones;
		dns.Definir(c.ip, auxNr);
		it.Avanzar();
	}

	// rearma las conexiones
	it = compus.CrearIt();
	while(it.HaySiguiente()) {
	 	Compu c = it.Siguiente();
	 	NodoRed nodoMio = dns.Significado(c.ip);
	 	NodoRed nodoOtro = otro.dns.Significado(c.ip);

	 	Dicc <Interfaz, NodoRed*>::Iterador itInterfs = nodoMio.conexiones.CrearIt();

	 	while(itInterfs.HaySiguiente()) {

	 		Interfaz interf = itInterfs.SiguienteClave();
	 		String ip = nodoOtro.conexiones.Significado(interf)->pc.ip;
	 		nodoMio.conexiones.Definir(interf, &dns.Significado(ip));
			itInterfs.Avanzar();
	 	}
	 	it.Avanzar();
	}

}

Conj<Compu> Red::Computadoras () {
	return compus;
}

void Red::AgregarComputadora(const Compu& c) {
	// PRE: c no está en la red

	NodoRed nr(c);

	compus.AgregarRapido(c);

	// inicializarConjcaminos esta embebida en este loop
	Dicc<String, NodoRed>::Iterador it = dns.CrearIt();
	while(it.HaySiguiente()) {
		NodoRed& nrt = it.SiguienteSignificado();
		nrt.caminos.Definir(nr.pc.ip, Conj<Camino>());
		nr.caminos.Definir(nrt.pc.ip, Conj<Camino>());
		it.Avanzar();
	}

	dns.Definir(c.ip, nr);
}

void Red::Conectar(const Compu& c1, const Compu& c2, const int i1, const int i2) {
	// PRE: las interfaces de esas compus existen y estan libres

	NodoRed& n1 = dns.Significado(c1.ip);
	NodoRed& n2 = dns.Significado(c2.ip);
	n1.conexiones.Definir(i1, &n2);
	n2.conexiones.Definir(i2, &n1);

	//cout << "Conectadas " << c1.ip << "(" << i1 << ") con " << c2.ip << "(" << i2 << ")" << endl;
	crearTodosLoscaminos();
}



void Red::crearTodosLoscaminos () {
	Conj<Compu>::Iterador itCompuA = compus.CrearIt();

	while (itCompuA.HaySiguiente()) {
		NodoRed* nr = &dns.Significado(itCompuA.Siguiente().ip);

		Conj<Compu>::Iterador itCompuB = compus.CrearIt();
		// cout << "creo los caminos de ";
		// cout << nr->pc.ip << endl;
		while (itCompuB.HaySiguiente()) {
			Computadora ipDestino = itCompuB.Siguiente().ip;

			Conj<Camino> caminimos = minimos(caminos(*nr, ipDestino));
			nr->caminos.Definir(ipDestino, caminimos);

			itCompuB.Avanzar();
		}
		// cout << nr->caminos  << endl;

		itCompuA.Avanzar();
	}
}

Conj<Camino> Red::caminos (const NodoRed& c1, const Computadora& ipDestino) {
	Conj<Camino> res;
	//cout << "Buscando caminos de " << c1.pc.ip << " a " << ipDestino << endl;

	Pila< Lista<NodoRed> > frameRecorrido;
	Pila< Lista<NodoRed> > frameCandidatos;

	Lista<NodoRed> iCandidatos = agregarListaNodosVecinos(c1, Lista<NodoRed>());
	Lista<NodoRed> iRecorrido;
	iRecorrido.AgregarAdelante(c1.pc);

	frameRecorrido.Apilar(iRecorrido);
	frameCandidatos.Apilar(iCandidatos);
	// cout << ipDestino<< endl;
	// cout << "candidatos: " << compusDeNodos(iCandidatos) << endl;
	// cout << "recorrido: " << compusDeNodos(iRecorrido) << endl;
	// Compu pCandidatos; movido abajo para simplificar.

	Lista<NodoRed> fCandidatos;

	while(!frameRecorrido.EsVacia()) {
		//cout << " desapilando:" << endl;
		iRecorrido = frameRecorrido.Tope();
		iCandidatos = frameCandidatos.Tope();
		frameRecorrido.Desapilar();
		frameCandidatos.Desapilar();

		if (iRecorrido.Ultimo().pc.ip == ipDestino) {
			//cout << "    camino encontrado! " << compusDeNodos(iRecorrido) << endl;
			res.AgregarRapido(compusDeNodos(iRecorrido));
			continue;
		}

		if (iCandidatos.EsVacia()) {
			continue;
		}

		NodoRed primerCandidato = iCandidatos.Primero();

		iCandidatos.Fin();
		fCandidatos = iCandidatos;

		frameRecorrido.Apilar(iRecorrido);
		frameCandidatos.Apilar(fCandidatos);

		if (!nodoEnLista(primerCandidato, iRecorrido)) {
			iRecorrido.AgregarAtras(primerCandidato);
			frameRecorrido.Apilar(iRecorrido);
			frameCandidatos.Apilar(agregarListaNodosVecinos(primerCandidato, fCandidatos));
		}
	}

	return res;
}

// originalmetnte no existia
Lista<Compu> Red::compusDeNodos (const Lista<NodoRed>& ns) {
	Lista<Compu> res;
	Lista<NodoRed>::const_Iterador itNodos = ns.CrearIt();
	while (itNodos.HaySiguiente()) {
		res.AgregarAtras(itNodos.Siguiente().pc);
		itNodos.Avanzar();
	}
	return res;
}

Conj<Camino> Red::minimos (const Conj<Camino>& caminos) {
	Conj<Camino> res;
	Nat longMinima;
	Conj<Camino>::const_Iterador itcaminos = caminos.CrearIt(); // agregado const_
	if (itcaminos.HaySiguiente()) {
		longMinima = itcaminos.Siguiente().Longitud();
		itcaminos.Avanzar();
		while(itcaminos.HaySiguiente()) {
			if (itcaminos.Siguiente().Longitud() < longMinima) {
				longMinima = itcaminos.Siguiente().Longitud();
			}
			itcaminos.Avanzar();
		}
		itcaminos = caminos.CrearIt();
		while(itcaminos.HaySiguiente()) {
			if (itcaminos.Siguiente().Longitud() == longMinima) {
				res.AgregarRapido(itcaminos.Siguiente());
			}
			itcaminos.Avanzar();
		}
	}
	return res;
}

Lista<NodoRed> Red::agregarListaNodosVecinos (const NodoRed& n, const Lista<NodoRed>& original) {
	Lista<NodoRed> res = original;
	Dicc<Interfaz, NodoRed*>::const_Iterador itVecinos = n.conexiones.CrearIt(); // agregado const_
	while (itVecinos.HaySiguiente()) {
		res.AgregarAdelante(*itVecinos.SiguienteSignificado());
		itVecinos.Avanzar();
	}
	return res;
}

bool Red::nodoEnLista (const NodoRed& n, const Lista<NodoRed>& ns) {
	Lista<NodoRed>::const_Iterador itNodos = ns.CrearIt(); // agregado el const_
	while (itNodos.HaySiguiente()) {
		if (itNodos.Siguiente() == n) {
			return true;
		};
		itNodos.Avanzar();
	}
	return false;
}


bool Red::UsaInterfaz( const Compu& c, const int i) {
	return dns.Significado(c.ip).conexiones.Definido(i);
}

int Red::InterfazUsada (const Compu& c1, const Compu& c2) {
	// PRE: c2 esta conectada a alguna interfaz de c1

	NodoRed* n1 = &dns.Significado(c1.ip);

	Dicc <Interfaz, NodoRed*>::Iterador it = n1->conexiones.CrearIt();

	while (it.HaySiguiente()) {
		NodoRed* n2 = it.SiguienteSignificado();
		if (c2.ip == n2->pc.ip) {
			return it.SiguienteClave();
		}
		it.Avanzar();
	}

	return -1; // esto no deberia alcanzarse por la PRE
}

Conj<Compu> Red::Vecinos (const Compu& c) {
	// PRE: c esta en la red
	Conj<Compu> res;
	Dicc<Interfaz, NodoRed*>::const_Iterador it = dns.Significado(c.ip).conexiones.CrearIt();
	while(it.HaySiguiente()) {
		res.AgregarRapido(it.SiguienteSignificado()->pc);
		it.Avanzar();
	}
	return res;
}

bool Red::HayCamino( const Compu& c1, const Compu& c2){
	NodoRed& nr = dns.Significado(c1.ip);
	return !(nr.caminos.Significado(c2.ip).EsVacio());

}

Conj< Camino > Red::CaminosMinimos( const Compu& c1, const Compu& c2){
	return dns.Significado(c1.ip).caminos.Significado(c2.ip);
};

bool Red::Conectadas( const Compu& c1, const Compu& c2) {
	//TODO: esto esta bien? no tiene que chequear que haya algún camino?
	// NO porque si estan Conectadas -> existe un camino , osea estan al lado mano son vecinas osea entende amigou?
	bool res = false;
	Dicc <Interfaz, NodoRed*>::Iterador it = dns.Significado(c1.ip).conexiones.CrearIt();

	while(it.HaySiguiente()) {
		NodoRed* nr =  it.SiguienteSignificado();
		if (c2.ip == (nr->pc.ip)) {
			res = true;
		}
		it.Avanzar();
	}
	return res;
}



bool Red::operator==(const Red& otro) const{
	return ( (otro.dns == dns) && (otro.compus == compus)  ) ;
}


Conj<Camino> agregarCompuAlPrimerCaminoDeConjuntoDeCaminos(Compu c, Conj<Camino> cc) {
	Conj<Camino> res;
	Camino cam = cc.CrearIt().Siguiente();
	cam.AgregarAtras(c);
	res.Agregar(cam);
	return res;
}