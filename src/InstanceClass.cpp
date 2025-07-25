#include "InstanceClass.h"

#include "Heap.h"
#include "Object.h"
#include "ClassVisao.h"
#include "MethodArea.h"
#include "Utils.h"

InstanceClass::InstanceClass(StaticClass *classRuntime) :
		_classRuntime(classRuntime) {
	ClassFile *classFile = classRuntime->getClassFile();
	field_info *fields = classFile->fields;
	u2 abstractFlag = 0x0400;

	if ((classFile->access_flags & abstractFlag) != 0) { // Não pode instanciar se for classe abstrata (i.e., interface)
		cerr << "InstantiationError" << endl;
		exit(1);
	}

	for (int i = 0; i < classFile->fields_count; i++) {
		const field_info &field = fields[i];
		u2 staticAndFinalFlag = 0x0008 | 0x0010;

		if ((field.access_flags & staticAndFinalFlag) == 0) { // não estática e não final
			string fieldName = Utils::getFormattedConstant(classFile->constant_pool, field.name_index);
			string fieldDescriptor = Utils::getFormattedConstant(classFile->constant_pool, field.descriptor_index);

			char fieldType = fieldDescriptor[0];
			Value value;

			switch (fieldType) {
			case 'B':
				value.type = ValueType::BYTE;
				value.data.byteValue = 0;
				break;
			case 'C':
				value.type = ValueType::CHAR;
				value.data.charValue = 0;
				break;
			case 'D':
				value.type = ValueType::DOUBLE;
				value.data.doubleValue = 0;
				break;
			case 'F':
				value.type = ValueType::FLOAT;
				value.data.floatValue = 0;
				break;
			case 'I':
				value.type = ValueType::INT;
				value.data.intValue = 0;
				break;
			case 'J':
				value.type = ValueType::LONG;
				value.data.longValue = 0;
				break;
			case 'S':
				value.type = ValueType::SHORT;
				value.data.shortValue = 0;
				break;
			case 'Z':
				value.type = ValueType::BOOLEAN;
				value.data.charValue = false;
				break;
			default:
				value.type = ValueType::REFERENCE;
				value.data.object = NULL;
			}

			putValueIntoField(value, fieldName);
		}
	}

	// quando um objeto é criado, ele é armazenado na Heap
	Heap &heap = Heap::getInstance();
	heap.addObject(this);
}

InstanceClass::~InstanceClass() {

}

ObjectType InstanceClass::objectType() {
	return ObjectType::CLASS_INSTANCE;
}

StaticClass* InstanceClass::getClassRuntime() {
	return _classRuntime;
}

void InstanceClass::putValueIntoField(Value value, const string& fieldName) {
	_fields[fieldName] = value;
}

Value InstanceClass::getValueFromField(string fieldName) {
	map<string, Value>::iterator it = _fields.find(fieldName);
       if (it == _fields.end()) {
               cerr << "NoSuchFieldError" << endl;
               exit(1);
       }
	return it->second;
}

bool InstanceClass::fieldExists(string fieldName) {
	return _fields.find(fieldName) != _fields.end();
}
