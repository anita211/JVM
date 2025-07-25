#ifndef frame_h
#define frame_h

#include <iostream>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <stack>
#include <map>

#include "BasicTypes.h"
#include "InstanceClass.h"
#include "StaticClass.h"

using namespace std;

/** @class Frame
 * @brief Estrutura de armazenamento.
 * @brief Armazenar dados e resultados parciais, executar ligação dinâmica, retornar valores para métodos e disparar exceções.
 */
class Frame {

public:
	/**
	 * @brief Construtor utilizado quando o frame está associado a um método de instância.
	 * @param object O objeto que contém o método do frame que será criado.
	 * @param classRuntime A classe que contém o método do frame que será criado.
	 * @param methodName O nome do método referente ao frame atual.
	 * @param methodDescriptor O descritor do método referente ao frame atual.
	 * @param arguments Os argumentos do método.
	 */
	Frame(InstanceClass *object, StaticClass *classRuntime, const string& methodName, const string& methodDescriptor, const vector<Value>& arguments);

	/**
	 * @brief Construtor utilizado quando o frame está associado a um método estático.
	 * @param classRuntime A classe que contém o método do frame que será criado.
	 * @param methodName O nome do método referente ao frame atual.
	 * @param methodDescriptor O descritor do método referente ao frame atual.
	 * @param arguments Os argumentos do método.
	 */
	Frame(StaticClass *classRuntime, const string& methodName, const string& methodDescriptor, const vector<Value>& arguments = vector<Value>());

	/**
	 * @brief Copy constructor.
	 */
	Frame(const Frame& other);

	/**
	 * @brief Assignment operator.
	 */
	Frame& operator=(const Frame& other);

	/**
	 * @brief Destrutor padrão - Libera recursos associados (se houver), garantindo limpeza correta. .
	 */
	~Frame();

	/**
	 * @brief Retorna um ponteiro para o array de entradas da Constant Pool do método em execução. 
	 * @return Retorna o ponteiro para a pool de constantes.
	 */
	cp_info** obterConstantPool();

	/**
	 * @brief Lê o valor na posição index (verifica limites).
	 *
	 * Se o índice for inválido, é emitido um erro.
	 * @param index O índice da variável local.
	 * @return O valor da variável local.
	 */
	Value obterLocalVariableValue(uint32_t index);

	/**
	 * @brief Modifica o valor de uma variável local.
	 *
	 * Se o valor for inválido (não ser do mesmo tipo existente), é emitido um erro.
	 * @param variableValue O novo valor que será colocado na variável local.
	 * @param index O índice da variável local que será modificada.
	 */
	void trocaLocalVariable(Value variableValue, uint32_t index);

	/**
	 * @brief Adiciona um operando na pilha de operandos.
	 * @param operand O operando que será inserido
	 */
	void empilharOperandStack(Value operand);

	/**
	 * @brief Remove o operando do topo da pilha de operandos.
	 *
	 * A pilha necessita ter ao menos um elemento. Caso não tenha, um erro é emitido.
	 * @return O valor do operando removido da pilha
	 */
	Value desempilhaOperandStack();

	/**
	 * @brief Realiza uma cópia da pilha de operandos.
	 * @return Retorna uma cópia da pilha de operandos do Frame.
	 */
	stack<Value> backupOperandStack();

	/**
	 * @brief Troca a pilha de operandos atual por uma nova.
	 * @param backup Uma nova pilha de operandos do Frame.
	 */
	void setOperandStackFromBackup(const stack<Value>& backup);

	/**
	 * @brief Obtém o objeto associado ao frame, quando existir.
	 * @return O objeto associado ao frame. Caso for um método estático, é retornado \c NULL.
	 */
	InstanceClass* getObject();

	/**
	 * @brief Obtém o código do método a partir de um offset.
	 * @param address O offset.
	 * @return O código a partir do offset dado.
	 */
	u1* getCode(uint32_t address);

	/**
	 * O program counter do frame.
	 */
	u4 pc;

	/**
	 * @brief Retorna o comprimento do vetor de variáveis locais associado ao frame.
	 * @return O conteúdo de _codeAttribute->max_locals
	 */
	u2 sizeLocalVariables();

	/**
	 * @brief Retorna o comprimento do código do método não-nativo em execução.
	 * @return O conteúdo de _codeAttribute->code_length
	 */
	u4 sizeCode();

private:
	/**
	 * @brief Obtém um método a partir da classe informada ou de alguma super classe.
	 *
	 * Esse método pode realizar o carregamento de novas classes.
	 * @param classRuntime A classe em que a pesquisa pelo métood será iniciada.
	 * @param name O nome do método.
	 * @param descriptor O descritor do método.
	 * @return Um ponteiro para o método encontrado, ou \c NULL caso o método não exista.
	 */
	method_info* obterMethodNamed(StaticClass *classRuntime, string name, string descriptor);

	/**
	 * @brief Encontra os atributos Code e Exceptions do método associado ao frame.
	 *
	 * O atributo Code será armazenado em \c _codeAttribute.
	 * O atributo Exceptions, caso exista, será armazenado em \c _exceptionsAttribute, caso não existir, seu valor será setado para \c NULL.
	 */
	void encontrarAttributes();

	/**
	 * @brief Ponteiro para a classe associada ao frame.
	 * @return Retorna um ponteiro para a classe.
	 */
	StaticClass *_classRuntime;

	/**
	 * Ponteiro para o objeto responsável por o método referente a este frame. Caso o método seja estático, seu valor é \c NULL.
	 */
	InstanceClass *_object;

	/**
	 * Ponteiro para o método referente a este frame.
	 */
	method_info _method;

	/**
	 * Ponteiro para o atributo Code referente ao método.
	 */
	Code_attribute *_codeAttribute;

	/**
	 * Atributo Exceptions referente ao método.
	 */
	Exceptions_attribute *_exceptionsAttribute;

	/**
	 * Array de variáveis locais do frame (alocado dinamicamente).
	 */
	Value* _localVariables;

	/**
	 * Ponteiro para a pilha de operandos do frame (alocado dinamicamente).
	 */
	stack<Value>* _operandStack;

};

#endif /* frame_h */