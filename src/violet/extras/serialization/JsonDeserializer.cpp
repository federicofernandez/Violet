#include "violet/extras/serialization/JsonDeserializer.h"

#include <fstream>
#include <memory>
#include <algorithm>

using namespace Violet;

JsonDeserializer::JsonDeserializer(std::istream & stream) :
	m_root(),
	m_valid(),
	m_stack()
{
	Json::Reader reader;
	m_valid = reader.parse(stream, m_root, false);
	if (m_valid)
		m_stack.push_front(std::make_pair(&m_root, 0));
}

JsonDeserializer::JsonDeserializer(JsonDeserializer && other) :
	m_root(std::move(other.m_root)),
	m_valid(other.m_valid),
	m_stack(std::move(other.m_stack))
{
}

JsonDeserializer::operator bool() const
{
	return m_valid && !m_stack.empty();
}

void JsonDeserializer::enterSegment(const char * label)
{
	if (label != nullptr)
		m_stack.emplace_front(&m_stack.front().first->operator[](label), 0);
	else
		m_stack.emplace_front(&m_stack.front().first->operator[](m_stack.front().second++), 0);
}

void JsonDeserializer::leaveSegment()
{
	m_stack.pop_front();
}

bool JsonDeserializer::getBoolean(const char * label)
{
	return m_stack.front().first->get(label, false).asBool();
}

uint32 JsonDeserializer::getUint(const char * label)
{
	return m_stack.front().first->get(label, 0).asUInt();
}

int JsonDeserializer::getInt(const char * label)
{
	return m_stack.front().first->get(label, 0).asInt();
}

float JsonDeserializer::getFloat(const char * label)
{
	return static_cast<float>(getDouble(label));
}

double JsonDeserializer::getDouble(const char * label)
{
	return m_stack.front().first->get(label, 0).asDouble();
}

const char * JsonDeserializer::getString(const char * label)
{
	return m_stack.front().first->get(label, "").asCString();
}
