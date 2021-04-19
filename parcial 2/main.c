void invertir(abb_t* arbol){
	if (arbol == NULL){return;}
	if (arbol->izq == NULL && arbol->der == NULL){return;}
	invertir(arbol->der);
	invertir(arbol->izq);
	abb_t* aux = arbol->der;
	arbol-der = arbol->izq;
	arbol->izq = arbol->der;
	return;
}
