#include "HNProtocol.h"
#include "../leak_detector/leak_detector_c.h"


// xml
XmlElement client_send_video_req[] = { 
	{XML_ROOT,			"Message" },
	{XML_ELEMENT,		"Session",		{ "" }, { "" },	"fdsgsdfdsfsadfsdfasdga777",	{ "Message" }},
	{XML_ELEMENT,		"ClientType",	{ "" },	{ "" },	"1",	{ "Message" }},
	{XML_EOF}
};

XmlElement client_recv[] = {
	{XML_ROOT,			"Message" },
	{XML_ELEMENT,		"Error",		{ "" }, { "" },	"555",	{ "Message" }},
	{XML_ELEMENT,		"Session",		{ "" },	{ "" },	"666",	{ "Message" }},
	{XML_EOF}
};


//////////////////////////////////////////////////////////////////////////
HNProtocol::HNProtocol()
{

}

HNProtocol::~HNProtocol()
{

}


XML_Char* HNProtocol::CreateXmlBuffer(XmlElement *all_elements)
{
	scew_tree		*xml_tree			= NULL;
	scew_element	*xml_root			= NULL;
	scew_element	*xml_element		= NULL;
	
	/**
	* Create an empty XML tree in memory, and add a root element
	* "scew_test".
	*/
	xml_tree = scew_tree_create ();
#ifdef XML_UNICODE_WCHAR_T
	scew_tree_set_xml_encoding(tree, _XT("UTF-16"));
#endif /* XML_UNICODE_WCHAR_T */
	if (NULL == xml_tree) {
		return NULL;
	}
	int element_count = 0;
	for (element_count =0; XML_EOF != all_elements[element_count].element_deep; element_count++)
	{
		all_elements[element_count].cur_parent_key_index = 0;
		switch (all_elements[element_count].element_deep) {
		case XML_ROOT:
			if (NULL != all_elements[element_count].element_key && 0 == element_count) {
				xml_root = scew_tree_set_root (xml_tree, _XT(all_elements[element_count].element_key));
			} else {
				printf("error: not found xml root\n");
				return NULL;
			}
			break;
		case XML_ELEMENT:
		case XML_SUB_ELEMENT:
			AddFrameInfoToXml(xml_tree, &all_elements[element_count]);
			break;
		case XML_EOF:
			break;
		}
	}
	
	XML_Char *ret = SaveXmlIntoBuffer(xml_tree);
	// free other memory
	/* Remember to free tree (scew_parser_free does not free it). */
	if (NULL != xml_tree) {
		scew_tree_free (xml_tree);
		xml_tree = NULL;
	}

	return ret;
}

int HNProtocol::AddFrameInfoToXml(scew_tree *xml_tree, XmlElement *pelement)
{
	int ret = 0;
	scew_attribute *attribute = NULL;

	if (NULL == pelement) {
		return ret;
	}
	/* Add an element and set element contents. */
	AddXmlElement_(scew_tree_root(xml_tree), pelement, ALL_OF_KIND);

	ret = 1;
	return ret;
}

XML_Char* HNProtocol::SaveXmlIntoBuffer(scew_tree *m_xml_tree, int buf_size)
{
	scew_printer	*xml_printer		= NULL;
	scew_writer		*xml_writer			= NULL;

	/* Save the XML tree to a buffer and print it to standard output. */
	XML_Char *xml_buffer = (XML_Char *) malloc (buf_size);
	xml_writer = scew_writer_buffer_create (xml_buffer, buf_size);
	if (NULL == xml_writer)
	{
		if (NULL != xml_buffer) {
			free(xml_buffer);
			xml_buffer = NULL;
		}
		printf ("Unable to create writer buffer\n");
		return xml_buffer;
	}
	xml_printer = scew_printer_create (xml_writer);
	if (NULL == xml_printer)
	{
		if (NULL != xml_buffer) {
			free(xml_buffer);
			xml_buffer = NULL;
		}
		printf ("Unable to create printer\n");
		return xml_buffer;
	}
	/* We should check for errors here. */
	(void) scew_printer_print_tree(xml_printer, m_xml_tree);
	if (NULL != xml_writer) {
		scew_writer_free (xml_writer);
	}
	if (NULL != xml_printer) {
		scew_printer_free (xml_printer);
	}

	return xml_buffer;
}

static XML_Char* save_xml_into_buffer(scew_tree *xml_tree, int buf_size)
{
	scew_writer *xml_writer = NULL;
	scew_printer *xml_printer = NULL;

	/* Save the XML tree to a buffer and print it to standard output. */
	XML_Char *xml_buffer = (XML_Char *) malloc (buf_size);
	xml_writer = scew_writer_buffer_create (xml_buffer, buf_size);
	if (NULL == xml_writer)
	{
		if (NULL != xml_buffer) {
			free(xml_buffer);
			xml_buffer = NULL;
		}
		printf ("Unable to create writer buffer\n");
		return xml_buffer;
	}
	xml_printer = scew_printer_create (xml_writer);
	if (NULL == xml_printer)
	{
		if (NULL != xml_buffer) {
			free(xml_buffer);
			xml_buffer = NULL;
		}
		printf ("Unable to create printer\n");
		return xml_buffer;
	}
	/* We should check for errors here. */
	(void) scew_printer_print_tree(xml_printer, xml_tree);

	scew_writer_free (xml_writer);
	scew_printer_free (xml_printer);

	return xml_buffer;
}

void HNProtocol::PrintXmlBuffer(XML_Char *xml_buffer)
{
	// print xml buffer
	if (NULL == xml_buffer) {
		scew_printf (_XT("%s"), "null");
	} else {
		scew_printf (_XT("%s"), xml_buffer);
	}
}

int HNProtocol::AddXmlElement_(scew_element *element, XmlElement *add_element, int add_kind)
{
	int ret = 0;
	XML_Char const *element_key = NULL;
	XML_Char const *contents = NULL;

	unsigned int indent = 0;

	scew_list *list = NULL;

	if (element == NULL || NULL == add_element)
	{
		return ret;
	}
	if (NULL == add_element->element_key || 0 == strcmp(add_element->element_key, "") ||
		NULL == *add_element->parent_key || 0 == strcmp(*add_element->parent_key, "")) {
		return ret;
	}
	if (NULL == add_element->cur_parent_key) {
		add_element->cur_parent_key = *add_element->parent_key;
		add_element->cur_parent_key_index = 0;
	}

	/* Prints the starting element tag with its attributes. */
	element_key = scew_element_name(element);
	//print_attributes (element);
	contents = scew_element_contents(element);
	// compare key_name is right
	list = scew_element_children(element);
	scew_attribute *attribute = NULL;
	if (0 == strcmp(element_key, add_element->cur_parent_key)) {
		if (NULL == list) {
			AddXmlElement_2(element, add_element, add_kind);
			add_element->cur_parent_key_index = -1;
		}
		if (-1 == add_element->cur_parent_key_index) {
			ret = 1;
			return ret;
		}
		add_element->cur_parent_key_index++;
		if (0 <= add_element->cur_parent_key_index &&
			NULL != add_element->parent_key[add_element->cur_parent_key_index]) {
			add_element->cur_parent_key = add_element->parent_key[add_element->cur_parent_key_index];
		} else {
			if (0 <= add_element->cur_parent_key_index) {
				add_element->cur_parent_key = add_element->element_key;
				add_element->cur_parent_key_index = -1;
			} else {
				ret = 1;
				return ret;
			}
		}
	}

	/**
	* Call print_element function again for each child of the current
	* element.
	*/
	while (list != NULL)
	{
		scew_element *child = (scew_element *)scew_list_data(list);
		ret = AddXmlElement_(child, add_element, add_kind);
		if (1 == ret) {
			// add element into buffer
			/* Add an element and set element contents. */
			if (-2 == add_element->cur_parent_key_index || NULL == child) {
				return ret;
			}
			AddXmlElement_2(child, add_element, add_kind);
			add_element->cur_parent_key_index = -2;
			break;
		}
		indent++;
		list = scew_list_next (list);
		if (NULL == list && 0 == ret && (-1 == add_element->cur_parent_key_index || 0 == add_element->cur_parent_key_index)) {
			AddXmlElement_2(element, add_element, add_kind);
			add_element->cur_parent_key_index = -2;
			ret = 1;
			return ret;
		}
	}
	
	return ret;
}

int HNProtocol::AddXmlElement_2(scew_element *element, XmlElement *add_element, int add_kind)
{
	int ret = 0;
	scew_attribute *attribute = NULL;

	switch(add_kind) {
	case SET_ELEMENT:
		if (NULL != add_element->key_attrib) {
			int key_attrib_count = 0;
			for (key_attrib_count = 0; ;key_attrib_count++) {
				if (NULL == add_element->key_attrib[key_attrib_count] ||
					0 == strcmp(add_element->key_attrib[key_attrib_count], "")) {
						break;
				}
				if (NULL == add_element->key_attrib_content[key_attrib_count]) {
					attribute = scew_attribute_create (_XT(add_element->key_attrib[key_attrib_count]), 
						_XT(""));
				} else {
					attribute = scew_attribute_create (_XT(add_element->key_attrib[key_attrib_count]), 
						_XT(add_element->key_attrib_content[key_attrib_count]));
				}
				
				scew_element_add_attribute (element, attribute);
			}
		}
		scew_element_set_contents (element, _XT(add_element->element_info));
		break;
	case ADD_ELEMENT:

		break;
	case ALL_OF_KIND:
		element = scew_element_add (element, _XT(add_element->element_key));
		if (NULL == element) {
			return ret;
		}
		if (NULL != add_element->key_attrib) {
			int key_attrib_count = 0;
			for (key_attrib_count = 0; ;key_attrib_count++) {
				if (NULL == add_element->key_attrib[key_attrib_count] ||
					0 == strcmp(add_element->key_attrib[key_attrib_count], "")) {
						break;
				}
				if (NULL == add_element->key_attrib_content[key_attrib_count]) {
					attribute = scew_attribute_create (_XT(add_element->key_attrib[key_attrib_count]), 
						_XT(""));
				} else {
					attribute = scew_attribute_create (_XT(add_element->key_attrib[key_attrib_count]), 
						_XT(add_element->key_attrib_content[key_attrib_count]));
				}
				scew_element_add_attribute (element, attribute);
			}
		}
		if (NULL == add_element->element_info) {
			scew_element_set_contents(element, _XT(""));
		} else {
			scew_element_set_contents(element, _XT(add_element->element_info));
		}
		add_element->cur_parent_key_index = -1;
		ret = 1;
		break;
	}

	return ret;
}

XML_Char* HNProtocol::AddElementInXmlBuffer(XML_Char *buffer_in, XmlElement *add_element, int add_kind)
{
	scew_reader *reader = NULL;
	scew_parser *parser = NULL;
	scew_tree *tree = NULL;

#if defined(_MSC_VER) && defined(XML_UNICODE_WCHAR_T)
	/* Change stdout to Unicode before writing anything. */
	_setmode(_fileno(stdout), _O_U16TEXT);
#endif /* _MSC_VER && XML_UNICODE_WCHAR_T */

	/* Creates an SCEW parser. This is the first function to call. */
	parser = scew_parser_create();

	scew_parser_ignore_whitespaces (parser, SCEW_TRUE);

	/* Loads an XML file. */
	reader = scew_reader_buffer_create(buffer_in, strlen(buffer_in));
	//reader = scew_reader_file_create("testXML.xml");
	if (reader == NULL)
	{
		scew_error code = scew_error_code ();
		scew_printf (_XT("Unable to load file (error #%d: %s)\n"),
			code, scew_error_string (code));
	}
	tree = scew_parser_load (parser, reader);
	if (tree == NULL)
	{
		scew_error code = scew_error_code ();
		scew_printf (_XT("Unable to parse file (error #%d: %s)\n"),
			code, scew_error_string (code));
		if (code == scew_error_expat)
		{
			enum XML_Error expat_code = scew_error_expat_code (parser);
			scew_printf (_XT("Expat error #%d (line %d, column %d): %s\n"),
				expat_code,
				scew_error_expat_line (parser),
				scew_error_expat_column (parser),
				scew_error_expat_string (expat_code));
		}

		/* Frees the SCEW parser and reader. */
		scew_reader_free (reader);
		scew_parser_free (parser);

		return buffer_in;
	}
	AddXmlElement_(scew_tree_root(tree), add_element, add_kind);
	// save xml to buf
	XML_Char *p_xml_buffer = NULL;
	p_xml_buffer = save_xml_into_buffer(tree, strlen(buffer_in));
	if (NULL != buffer_in) {
		free(buffer_in);
		buffer_in = NULL;
	}
	buffer_in = p_xml_buffer;
	p_xml_buffer = NULL;

	/* Remember to free tree (scew_parser_free does not free it). */
	scew_tree_free (tree);

	/* Frees the SCEW parser and reader. */
	scew_reader_free (reader);
	scew_parser_free (parser);

	return buffer_in;
}

void HNProtocol::GetElementInXmlBuffer(XML_Char *buffer_in, XmlElement *in_element)
{
	scew_reader *reader = NULL;
	scew_parser *parser = NULL;
	scew_tree *tree = NULL;

#if defined(_MSC_VER) && defined(XML_UNICODE_WCHAR_T)
	/* Change stdout to Unicode before writing anything. */
	_setmode(_fileno(stdout), _O_U16TEXT);
#endif /* _MSC_VER && XML_UNICODE_WCHAR_T */

	/* Creates an SCEW parser. This is the first function to call. */
	parser = scew_parser_create();

	scew_parser_ignore_whitespaces (parser, SCEW_TRUE);

	/* Loads an XML file. */
	reader = scew_reader_buffer_create(buffer_in, strlen(buffer_in));
	//reader = scew_reader_file_create("testXML.xml");
	// test show xml frame
	//show_xml_buffer(buffer_in);
	if (reader == NULL)
	{
		scew_error code = scew_error_code ();
		scew_printf (_XT("Unable to load file (error #%d: %s)\n"),
			code, scew_error_string (code));
	}
	tree = scew_parser_load (parser, reader);
	if (tree == NULL)
	{
		scew_error code = scew_error_code ();
		scew_printf (_XT("Unable to parse file (error #%d: %s)\n"),
			code, scew_error_string (code));
		if (code == scew_error_expat)
		{
			enum XML_Error expat_code = scew_error_expat_code (parser);
			scew_printf (_XT("Expat error #%d (line %d, column %d): %s\n"),
				expat_code,
				scew_error_expat_line (parser),
				scew_error_expat_column (parser),
				scew_error_expat_string (expat_code));
		}

		/* Frees the SCEW parser and reader. */
		scew_reader_free (reader);
		scew_parser_free (parser);

		return ;
	}
	for (int i = 0; XML_EOF != in_element[i].element_deep; i++) {
		GetXmlElement_(scew_tree_root(tree), &in_element[i]);
	}
	
	/* Remember to free tree (scew_parser_free does not free it). */
	scew_tree_free (tree);

	/* Frees the SCEW parser and reader. */
	scew_reader_free (reader);
	scew_parser_free (parser);

	return ;
}

int HNProtocol::GetXmlElement_(scew_element *element, XmlElement *in_element)
{
	int ret = 0;
	XML_Char const *element_key = NULL;
	XML_Char const *contents = NULL;

	unsigned int indent = 0;

	scew_list *list = NULL;


	if (element == NULL || NULL == in_element ||
		NULL == in_element->element_key || 0 == strcmp(in_element->element_key, "") ||
		NULL == *in_element->parent_key || 0 == strcmp(*in_element->parent_key, ""))
	{
		return ret;
	}
	if (NULL == in_element->cur_parent_key) {
		in_element->cur_parent_key = *in_element->parent_key;
		in_element->cur_parent_key_index = 0;
	}

	/* Prints the starting element tag with its attributes. */
	element_key = scew_element_name(element);
	//print_attributes (element);
	contents = scew_element_contents(element);
	// compare key_name is right
	list = scew_element_children(element);
	scew_attribute *attribute = NULL;
	if (0 == strcmp(element_key, in_element->cur_parent_key)) {
		if (-1 == in_element->cur_parent_key_index) {
			ret = 1;
			GetXmlElement_2(element, in_element);
			return ret;
		}
		in_element->cur_parent_key_index++;
		if (0 <= in_element->cur_parent_key_index &&
			NULL != in_element->parent_key[in_element->cur_parent_key_index]) {
			in_element->cur_parent_key = in_element->parent_key[in_element->cur_parent_key_index];
		} else {
			if (0 <= in_element->cur_parent_key_index) {
				in_element->cur_parent_key = in_element->element_key;
				in_element->cur_parent_key_index = -1;
			} else {
				ret = 1;
				GetXmlElement_2(element, in_element);
				return ret;
			}
		}
	}

	/**
	* Call print_element function again for each child of the current
	* element.
	*/
	while (list != NULL)
	{
		scew_element *child = (scew_element *)scew_list_data(list);
		ret = GetXmlElement_(child, in_element);
		if (1 == ret) {
			// get content
			//GetXmlElement_2(child, in_element);
			break;
		}
		indent++;
		list = scew_list_next (list);
	}
	
	return ret;
}

void HNProtocol::GetXmlElement_2(scew_element *element, XmlElement *out_element)
{
	XML_Char const *contents = NULL;
	if (NULL == element)
	{
		return ;
	}
	//element info (element);
	contents = scew_element_contents(element);
	if (NULL != contents) {
		out_element->element_info = (char *)malloc(strlen(contents) + 1);
		memcpy(out_element->element_info, contents, strlen(contents));
		out_element->element_info[strlen(contents)] = '\0';
	}
	// key attrib 
	unsigned int n_attribs = scew_element_attribute_count(element);
	unsigned int attribs_count = 0;
	if (0 < n_attribs) {
		scew_list *list = scew_element_attributes (element);
		while (list != NULL)
		{
			// attrib name
			scew_attribute *attribute = (scew_attribute *)scew_list_data(list);
			contents = scew_attribute_name(attribute);
			if (NULL != contents) {
				out_element->key_attrib[attribs_count] = (char *)malloc(strlen(contents) + 1);
				memcpy(out_element->key_attrib[attribs_count], contents, strlen(contents));
				out_element->key_attrib[attribs_count][strlen(contents)] = '\0';
			}
			// attrib info
			if (NULL != contents) {
				contents = scew_attribute_value(attribute);
				out_element->key_attrib_content[attribs_count] = (char *)malloc(strlen(contents) + 1);
				memcpy(out_element->key_attrib_content[attribs_count], contents, strlen(contents));
				out_element->key_attrib_content[attribs_count][strlen(contents)] = '\0';
			}
			list = scew_list_next (list);
			attribs_count++;
		}
	}
	
}
