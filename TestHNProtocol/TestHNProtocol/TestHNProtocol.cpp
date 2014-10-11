//// TestHNProtocol.cpp : 定义控制台应用程序的入口点。
////
//
//#include "stdafx.h"
//
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//


/**
 * @file     scew_write.c
 * @brief    SCEW usage example
 * @author   Aleix Conchillo Flaque <aleix@member.fsf.org>
 * @date     Sun Mar 30, 2003 12:21
 *
 * @if copyright
 *
 * Copyright (C) 2002-2009 Aleix Conchillo Flaque
 *
 * SCEW is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SCEW is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * @endif
 *
 * This example shows the usage of the SCEW API. It will create a new
 * XML and write it to a file.
 *
 * We will create an XML with the follwing structure:
 *
 *   <scew_test>
 *     <element>element contents</element>
 *     <element attribute="value"/>
 *     <element attribute1="value1" attribute2="value2"/>
 *     <element>
 *       <sub_element attribute="value"/>
 *       <sub_element attribute1="value1" attribute2="value2">
 *         <sub_sub_element attribute1="value1"
 *                          attribute2="new_value2"
 *                          attribute3="value3">
 *           With accents: à é è í ó ú
 *         </sub_sub_element>
 *       </sub_element>
 *     </element>
 *   </scew_test>
 */


#include "ProcessOcxXML.h"
#include "ProcessXML.h"
#include "./test/Client.h"
#include "./test/Server.h"
#include <cstdlib>
#include <ctime>

extern XmlElement client_send_video_req[];
extern XmlElement client_recv[];

#if defined(_MSC_VER) && defined(XML_UNICODE_WCHAR_T)
#include <fcntl.h>
#include <io.h>
#endif /* _MSC_VER && XML_UNICODE_WCHAR_T */

#include <stdio.h>

//enum { MAX_OUTPUT_BUFFER_ = 2000 };


void TestSendVideoRequest()
{	
	ProcessXML process_xml;
	
	srand((int)time(NULL));     //每次执行种子不同，生成不同的随机数
	int i = rand() % 10000;
	char info[4];

	//client_send_video_req[1].element_info = (char *)&i;

	XML_Char *p_xml_buf = process_xml.CreateXmlBuffer(client_send_video_req);
	SendMegHead send_data;
	send_data.xml_len = strlen(p_xml_buf);
	send_data.version = 100;
	send_data.cmd_code = CMD_CODE_SEND_VIDEO_REQUEST;
	send_data.source_id = 10000;
	send_data.dest_id = 7;
	send_data.data_len = sizeof(SendMegHead) + send_data.xml_len;

	/*XmlElement client_send_video_req11[] = { 
	{XML_ROOT,			"Message" },
	{XML_ELEMENT,		"Session",		{ "" }, { "" },	"",	{ "Message" }},
	{XML_ELEMENT,		"ClientType",	{ "" },	{ "" },	"",	{ "Message" }},
	{XML_EOF}
	};
	process_xml.GetElementInXmlBuffer(p_xml_buf, client_send_video_req11);*/

	client_main((char *)&send_data, sizeof(SendMegHead), p_xml_buf, NULL);

	//// 接收使用
	//SendMegHead request_data = process_xml.GetSendVideoRequest(NULL);
	//request_data.Release();
}

void TestRecvVideoRequest()
{
	ProcessXML process_xml;
	XML_Char *p_xml_buffer = NULL;

	                                  //取得区间[0,N)的整数
	p_xml_buffer = process_xml.CreateXmlBuffer(client_recv);
	RecvMegHead send_data;
	send_data.version = 100;
	send_data.cmd_code = CMD_CODE_RECV_VIDEO_REQUEST;
	send_data.success_flag = 0;
	server_main((char*)&send_data, sizeof(RecvMegHead), p_xml_buffer, NULL);

	//// 接收使用
	//ReceiveVideoReqData request_data = process_xml.GetRecvVideoRequest(NULL);
	//request_data.Release();
}

int
main(int argc, char *argv[])
{
	TestSendVideoRequest();

	return 0;
	
  ProcessOcxXML xml;

  scew_tree *tree = NULL;
  scew_element *root = NULL;
  scew_element *element = NULL;
  scew_element *sub_element = NULL;
  scew_element *sub_sub_element = NULL;
  scew_attribute *attribute = NULL;
  scew_printer *printer = NULL;
  scew_writer *writer = NULL;
  XML_Char *buffer = NULL;

#if defined(_MSC_VER) && defined(XML_UNICODE_WCHAR_T)
  /* Change stdout to Unicode before writing anything. */
  _setmode(_fileno(stdout), _O_U16TEXT);
#endif /* _MSC_VER && XML_UNICODE_WCHAR_T */

  if (argc < 2)
    {
      scew_printf (_XT("Usage: scew_write new_file.xml\n"));
      return EXIT_FAILURE;
    }

  /**
   * Create an empty XML tree in memory, and add a root element
   * "scew_test".
   */
  tree = scew_tree_create ();
#ifdef XML_UNICODE_WCHAR_T
  scew_tree_set_xml_encoding(tree, _XT("UTF-16"));
#endif /* XML_UNICODE_WCHAR_T */
  root = scew_tree_set_root (tree, _XT("test"));

  /* Add an element and set element contents. */
  element = scew_element_add (root, _XT("element"));
  scew_element_set_contents (element, _XT("element contents"));

  /* Add an element with an attribute pair (name, value). */
  element = scew_element_add (root, _XT("element"));
  scew_element_add_attribute_pair (element, _XT("attribute"), _XT("value"));

  element = scew_element_add (root, _XT("element"));
  scew_element_add_attribute_pair (element, _XT("attribute1"), _XT("value1"));

  /**
   * Another way to add an attribute. You loose attribute ownership,
   * so there is no need to free it.
   */
  attribute = scew_attribute_create (_XT("attribute2"), _XT("value2"));
  scew_element_add_attribute (element, attribute);

  element = scew_element_add (root, _XT("element"));
  sub_element = scew_element_add (element, _XT("subelement"));
  scew_element_add_attribute_pair (sub_element,
                                   _XT("attribute"), _XT("value"));

  sub_element = scew_element_add (element, _XT("subelement"));
  scew_element_add_attribute_pair (sub_element,
                                   _XT("attribute1"), _XT("value1"));
  scew_element_add_attribute_pair (sub_element,
                                   _XT("attribute2"), _XT("value2"));

  sub_sub_element = scew_element_add (sub_element, _XT("subsubelement"));
  scew_element_add_attribute_pair (sub_sub_element,
                                   _XT("attribute1"), _XT("value1"));
  scew_element_add_attribute_pair (sub_sub_element,
                                   _XT("attribute2"), _XT("value2"));
  scew_element_add_attribute_pair (sub_sub_element,
                                   _XT("attribute3"), _XT("value3"));
  /* Check attribute2 replacement. */
  scew_element_add_attribute_pair (sub_sub_element,
                                   _XT("attribute2"), _XT("new_value2"));
  scew_element_set_contents (sub_sub_element,
                             _XT("With accents: à é è í ó ú"));

  /* Save the XML tree to a file. */
  writer = scew_writer_file_create (argv[1]);
  if (writer == NULL)
    {
      printf ("Unable to create %s\n", argv[1]);
      return EXIT_FAILURE;
    }

  printer = scew_printer_create (writer);
  if (printer == NULL)
    {
      printf ("Unable to create printer\n");
      return EXIT_FAILURE;
    }

  /* We should check for errors here. */
  (void) scew_printer_print_tree (printer, tree);
  scew_writer_free (writer);

  /* Save the XML tree to a buffer and print it to standard output. */
  buffer = (XML_Char *) malloc (MAX_OUTPUT_BUFFER_);
  writer = scew_writer_buffer_create (buffer, MAX_OUTPUT_BUFFER_);
  if (writer == NULL)
    {
      printf ("Unable to create writer buffer\n");
      return EXIT_FAILURE;
    }
  /* Replace writer */
  scew_printer_set_writer (printer, writer);

  /* We should check for errors here. */
  (void) scew_printer_print_tree (printer, tree);

  scew_printf (_XT("%s"), buffer);
  free (buffer);

  /* Frees the SCEW tree, printer and writer. */
  scew_writer_free (writer);
  scew_printer_free (printer);
  scew_tree_free (tree);

  return 0;
}
