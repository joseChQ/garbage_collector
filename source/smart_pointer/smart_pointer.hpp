// Copyright 2020 Roger Peralta Aranibar
#ifndef SOURCE_SMART_POINTER_SMART_POINTER_HPP_
#define SOURCE_SMART_POINTER_SMART_POINTER_HPP_
#include <cstddef>
#include <map>
#include<iostream>
/*
 * Smart Pointer que implementa un tipo de estrategia por conteo de referencias.
 * Permite que varios SmartPointers puedan acceder al mismo recurso compartido.
 * Cuando el ˙ltimo puntero es eliminado, se elimina y libera el recurso.
 */
template <typename Type>
class SmartPointer {
 private:
  Type *resource_;
  static std::map<Type *,int> table;// en este objeto voy adminitrar la cantidad de referencias hacer type
 public:
  /* Constructor: SmartPointer(Type* resource=NULL)
   * Uso: SmartPointer<string> myPtr(new string);
   *      SmartPointer<string> myPtr;
   * ------------------------------------------------------------
   * Construye un nuevo objeto SmartPointer que administra el recurso
   * asignado. Se asume que el recurso viene de una llamada a new.
   * El recurso también podría ser NULL lo que ocasionaría que el
   * recurso no administre ningún recurso.
   */
  explicit SmartPointer(Type *resource) :resource_(resource) {
    if(resource_ != nullptr)
    table[resource]++;
  }
  SmartPointer(){};

  /* Destructor: ~SmartPointer();
   * Uso: (implícito)
   * ------------------------------------------------------------
   * Decrementa el contador de referencias del recurso, eliminando
   * y liberando la memoria si fuera el último SmartPointer apuntando
   * al recurso.
   */
  ~SmartPointer() {
    if(resource_!=nullptr)
    Detach();
  }

  /* SmartPointer operadores de "des-referencia"(dereference)
   * Uso: cout << *myPtr << endl;
   * Uso: cout << myPtr->length << endl;
   * ------------------------------------------------------------
   * Permite al SmartPointer comportarse como si fuera un puntero.
   */
  Type &operator*() const { return *resource_; }
  Type *operator->() const { return resource_; }

  /* Funciones de copia
   * Uso: SmartPointer<string> ptr=existingPointer;
   *      ptr=existingPtr;
   * ------------------------------------------------------------
   * Cambia el recurso referenciado por este SmartPointer por otro
   * SmartPointer. Si el conteo llega a cero, debe ser eliminado
   * (deallocated).
   */
  SmartPointer &operator=(const SmartPointer &other) {
    if(resource_!=other.resource_){
      if(other.resource_!=nullptr)
        table[other.resource_]++; 

      if(resource_!=nullptr )
        Detach();
      
      resource_ = other.resource_;
    }
    return *this;
  }
  SmartPointer &operator=(Type *other) {
    //std::cout<<"funcion"<<std::endl;
    if(other!=nullptr)
      table[other]++;
    if(resource_!=nullptr )
      Detach();
    
    resource_ = other;
    return *this;
  }
  SmartPointer(const SmartPointer &other) {
    //std::cout<<"copia"<<std::endl;
  }

  /* Helper Function: Obtener recurso.
   * Uso: Type* p=GetPointer();
   * ------------------------------------------------------------
   * Retorna una variable puntero al recurso administrado.
   */
  Type *GetPointer() const { return resource_; }

  /* Helper Function: Obtiene conteo
   * Uso: if (ptr.GetReferenceCount()==1) // Única referencia
   * ------------------------------------------------------------
   * Retorna el número de referencias apuntando al recurso.
   */
  size_t GetReferenceCount() const { return (table.count(resource_)?table[resource_]:0);}

  /* Helper Function: se des-asocia del recurso;
   * Uso: ptr.Detach();
   * ------------------------------------------------------------
   * Deja de administrar un recurso. eliminando y liberando la
   * memoria si es necesario.
   */
  void Detach() {
    table[resource_]--;
      if(!table[resource_])
      {
        Type *tmp = nullptr;
        tmp =resource_;
        table.erase(resource_);
        delete tmp;
      }
    resource_=nullptr;
  }
};
template<class T>
std::map<T*,int> SmartPointer<T>::table;
#endif  // SOURCE_SMART_POINTER_SMART_POINTER_HPP_