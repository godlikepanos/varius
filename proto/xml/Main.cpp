#include <iostream>
#include <tinyxml2.h>
#include <stdexcept>

#define ANKI_EXCEPTION(x) \
	std::runtime_error(std::string("") + x)

/// XXX
struct XmlElement
{
	friend class XmlDocument;
public:
	const char* getText() const
	{
		return el->GetText();
	}

	operator bool() const
	{
		return el != nullptr;
	}

	XmlElement& operator=(const XmlElement& b)
	{
		el = b.el;
		return *this;
	}

	XmlElement getChildElementOptional(const char* name)
	{
		XmlElement out;
		out.el = el->FirstChildElement(name);
		return out;
	}

	XmlElement getChildElement(const char* name)
	{
		XmlElement out = getChildElementOptional(name);
		if(!out)
		{
			throw ANKI_EXCEPTION("Cannot find <" + name + ">");
		}
		return out;
	}

	XmlElement getNextSiblingElement(const char* name)
	{
		XmlElement out;
		out.el = el->NextSiblingElement(name);
		return out;
	}

private:
	tinyxml2::XMLElement* el = nullptr;
};

/// XXX
class XmlDocument
{
public:
	void loadFile(const char* filename)
	{
		if(doc.LoadFile(filename))
		{
			throw ANKI_EXCEPTION("Cannot parse file");
		}
	}

	XmlElement getChildElement(const char* name)
	{
		XmlElement el;
		el.el = doc.FirstChildElement(name);
		return el;
	}

private:
	tinyxml2::XMLDocument doc;
};


void parseMaterialTag(XmlElement& mtlEl)
{
	// <renderingStage>
	XmlElement rsEl = mtlEl.getChildElement("renderingStage");
	int rs = std::stoi(rsEl.getText());

	// <levelsOfDetail>
	XmlElement lodEl = mtlEl.getChildElementOptional("levelsOfDetail");
	int lod = (lodEl) ? std::stoi(lodEl.getText()) : 1;

	// <shadow>
	// XXX

	// <shaderProgram>
	XmlElement sProgEl = mtlEl.getChildElement("shaderProgram");

	// <shader>
	XmlElement shaderEl = sProgEl.getChildElement("shader");
	while(shaderEl)
	{
		XmlElement typeEl = shaderEl.getChildElement("type");

		std::cout << typeEl.getText() << std::endl;

		shaderEl = shaderEl.getNextSiblingElement("shader");
	}

	std::cout << "rs: " << rs << " lod:" << lod << std::endl;
}

void loadMaterial(const char* filename)
{
	XmlDocument doc;

	doc.loadFile(filename);
	XmlElement rootEl = doc.getChildElement("material");

	parseMaterialTag(rootEl);
}

int main(int, char**)
{
	loadMaterial("test.xml");

	std::cout << "Bye!!" << std::endl;
	return 0;
}
