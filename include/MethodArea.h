#ifndef methodarea_h
#define methodarea_h

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>

#include <map>
#include <string>

#include "BasicTypes.h"
#include "StaticClass.h"

using namespace std;

/** @class MethodArea
 * @brief Classe responsável por todas as operações que gerenciam os métodos
 * @brief Essa classe é um singleton, ou seja, existe somente uma instância dela para cada instância da JVM.
 */
class MethodArea {

public:
	/**
	 * @brief Obter a instância única do MethodArea.
	 * @return A instância do MethodArea.
	 */
	static MethodArea& getInstance() {
		static MethodArea instance;
		return instance;
	}

	/**
	 * @brief Destrutor padrão.
	 */
	~MethodArea();

	/**
	 * @brief Carrega a classe com o nome dado e a adiciona na área de métodos.
	 * @param className O nome da classe (contendo o sufixo .class ou não).
	 * @result O ponteiro para \c ClassFile da classe carregada.
	 */
	StaticClass* carregarClassNamed(const string &className);

	/**
	 * @brief Busca por uma classe com o nome qualificado passado (e.g. java/lang/Object).
	 * @param className O nome qualificado da classe que será buscado.
	 * @return Um ponteiro para a classe será retornado caso ela exista, e \c NULL caso contrário.
	 */
	StaticClass* getClassNamed(const string &className);

private:
	/**
	 * @brief Construtor padrão.
	 */
	MethodArea();

	MethodArea(MethodArea const&);     // não permitir implementação do construtor de cópia
	void operator=(MethodArea const&); // não permitir implementação do operador de igual

	/**
	 * @brief Adiciona uma classe à área de métodos.
	 * @param classFile A classe que será adicionada.
	 * @return \c true caso a classe foi adicionada, e \c false caso contrário, pois uma classe com o mesmo nome já está adicionada.
	 */
	bool addClass(StaticClass *classFile);

	/**
	 * Um \c map contendo todas as classes presentes na área de métodos.
	 *
	 * A chave do map é o nome qualificado da classe, e o valor é um ponteiro para o \c ClassFile correspondente.
	 */
	map<string, StaticClass*> _classes;
};

#endif /* methodarea_h */
