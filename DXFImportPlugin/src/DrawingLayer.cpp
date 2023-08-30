#include "DrawingLayer.h"

#include "Utilities.h"

#include <IBK_MessageHandler.h>
#include <IBK_StringUtils.h>
#include <IBK_messages.h>

#include <tinyxml.h>


TiXmlElement * DrawingLayer::Block::writeXML(TiXmlElement * parent) const {
	if (m_id == INVALID_ID)  return nullptr;

	TiXmlElement * e = new TiXmlElement("Block");
	parent->LinkEndChild(e);

	if (m_id != INVALID_ID)
		e->SetAttribute("id", IBK::val2string<unsigned int>(m_id));
	if (m_color.isValid())
		e->SetAttribute("color", m_color.name().toStdString());
	if (!m_name.isEmpty())
		e->SetAttribute("name", m_name.toStdString());
	if (m_lineWeight > 0)
		e->SetAttribute("lineWeight", IBK::val2string<unsigned int>(m_lineWeight));

	return e;
}

void DrawingLayer::Block::readXML(const TiXmlElement *element){
	FUNCID(Drawing::Circle::readXMLPrivate);

	try {
		// search for mandatory attributes
		if (!TiXmlAttribute::attributeByName(element, "id"))
			throw IBK::Exception( IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
				IBK::FormatString("Missing required 'id' attribute.") ), FUNC_ID);

		// reading attributes
		const TiXmlAttribute * attrib = element->FirstAttribute();
		while (attrib) {
			const std::string & attribName = attrib->NameStr();
			if (attribName == "id")
				m_id = readPODAttributeValue<unsigned int>(element, attrib);
			else if (attribName == "lineWeight")
				m_lineWeight = readPODAttributeValue<unsigned int>(element, attrib);
			else if (attribName == "Name")
				m_name = QString::fromStdString(attrib->ValueStr());
			else if (attribName == "color")
				m_color.setNamedColor(QString::fromStdString(attrib->ValueStr()));
			else {
				IBK::IBK_Message(IBK::FormatString(XML_READ_UNKNOWN_ATTRIBUTE).arg(attribName).arg(element->Row()), IBK::MSG_WARNING, FUNC_ID, IBK::VL_STANDARD);
			}
			attrib = attrib->Next();
		}
	}
	catch (IBK::Exception & ex) {
		throw IBK::Exception( ex, IBK::FormatString("Error reading 'ZoneTemplate' element."), FUNC_ID);
	}
	catch (std::exception & ex2) {
		throw IBK::Exception( IBK::FormatString("%1\nError reading 'ZoneTemplate' element.").arg(ex2.what()), FUNC_ID);
	}
}


TiXmlElement * DrawingLayer::writeXML(TiXmlElement * parent) const {
	if (m_id == INVALID_ID)  return nullptr;
	TiXmlElement * e = new TiXmlElement("DrawingLayer");
	parent->LinkEndChild(e);

	if (m_id != INVALID_ID)
		e->SetAttribute("id", IBK::val2string<unsigned int>(m_id));
	if (!m_displayName.isEmpty())
		e->SetAttribute("displayName", m_displayName.toStdString());
	if (m_visible != DrawingLayer().m_visible)
		e->SetAttribute("visible", IBK::val2string<bool>(m_visible));
	if (m_color.isValid())
		e->SetAttribute("color", m_color.name().toStdString());
	e->SetAttribute("lineWeight", IBK::val2string<int>(m_lineWeight));
	if (m_idBlock != INVALID_ID)
		e->SetAttribute("idBlock", IBK::val2string<unsigned int>(m_idBlock));
	return e;
}

