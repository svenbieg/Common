//==================
// VariableHelper.h
//==================

#pragma once


//========
// Access
//========

template <class _var_t, class _value_t> _value_t VariableGet(_var_t* Variable, _value_t DefaultValue)
{
if(!Variable)
	return DefaultValue;
return Variable->Get();
}


//============
// Assignment
//============

template <class _var_t, class _value_t> VOID VariableAssign(_var_t** Variable, _value_t Value)
{
_var_t* pvar=*Variable;
if(pvar)
	{
	pvar->Set(Value);
	return;
	}
pvar=new _var_t(Value);
pvar->AddReference();
*Variable=pvar;
}

template <class _var_t, class _value_t> BOOL VariableFromString(_var_t* Variable, Handle<String> String, LPCSTR Format, BOOL Notify)
{
if(!String)
	return false;
_value_t value;
if(String->Scan(Format, &value)!=1)
	return false;
Variable->Set(value, Notify);
return true;
}


//============
// Comparison
//============

template <class _var_t, class _value_t> BOOL VariableEqual(_var_t* Variable, _value_t Value)
{
if(!Variable)
	return false;
return Variable->Get()==Value;
}

template <class _var_t, class _value_t> BOOL VariableEqual(_var_t* Variable, _value_t Value, _value_t Invalid)
{
if(!Variable)
	return false;
_value_t value=Variable->Get();
if(value==Invalid)
	return false;
return value==Value;
}

template <class _var_t, class _value_t> BOOL VariableAbove(_var_t* Variable, _value_t Value)
{
if(!Variable)
	return false;
return Variable->Get()>Value;
}

template <class _var_t, class _value_t> BOOL VariableAboveOrEqual(_var_t* Variable, _value_t Value)
{
if(!Variable)
	return false;
return Variable->Get()>=Value;
}

template <class _var_t, class _value_t> BOOL VariableBelow(_var_t* Variable, _value_t Value)
{
if(!Variable)
	return false;
return Variable->Get()<Value;
}

template <class _var_t, class _value_t> BOOL VariableBelowOrEqual(_var_t* Variable, _value_t Value)
{
if(!Variable)
	return false;
return Variable->Get()<=Value;
}
