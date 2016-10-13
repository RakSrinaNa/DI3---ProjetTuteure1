/*
 * Copyright 2010 Sébastien Aupetit <sebastien.aupetit@univ-tours.fr>
 *
 * This source code is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this source code. If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id: DocumentRowListUnit.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <DocumentRowList.h>
#include <UnitTest.h>
#include <sys/stat.h>
#include <sys/types.h>

static void test_DocumentRowList_init(void)
{
  DocumentRow row;

  DocumentRow_init(&row);
  ASSERT_EQUAL_STRING(row.code, "");
  ASSERT_EQUAL_STRING(row.designation, "");
  ASSERT_EQUAL_STRING(row.unity, "");
  ASSERT_EQUAL(row.next, NULL);
  DocumentRow_finalize(&row);
}

static void test_DocumentRowList_create(void)
{
  DocumentRow * row;

  row = DocumentRow_create();
  ASSERT_EQUAL_STRING(row->code, "");
  ASSERT_EQUAL_STRING(row->designation, "");
  ASSERT_EQUAL_STRING(row->unity, "");
  ASSERT_EQUAL(row->next, NULL);
  DocumentRow_destroy(row);
}

static void test_DocumentRowList_readAndWriteRow(void)
{
  DocumentRow * row;
  FILE * file;

  file = fopen(BASEPATH "/unittest/documentrowlist-unittest.db", "w+b");
  row = DocumentRow_create();
  row->basePrice = 1;
  DocumentRow_writeRow(row, file);
  row->basePrice = 2;
  DocumentRow_writeRow(row, file);
  row->basePrice = 3;
  DocumentRow_writeRow(row, file);
  DocumentRow_destroy(row);

  fseek(file, 0, SEEK_SET);
  row = DocumentRow_readRow(file);
  ASSERT_EQUAL_DOUBLE(row->basePrice, 1);
  DocumentRow_destroy(row);
  row = DocumentRow_readRow(file);
  ASSERT_EQUAL_DOUBLE(row->basePrice, 2);
  DocumentRow_destroy(row);
  row = DocumentRow_readRow(file);
  ASSERT_EQUAL_DOUBLE(row->basePrice, 3);
  DocumentRow_destroy(row);

  fclose(file);
}

static void test_DocumentRowList_generic(void)
{
  DocumentRow * list;
  DocumentRow * row;

  DocumentRowList_init(&list);
  row = DocumentRow_create();
  row->basePrice = 1;
  DocumentRowList_pushBack(&list, row);
  row = DocumentRow_create();
  row->basePrice = 2;
  DocumentRowList_pushBack(&list, row);
  row = DocumentRow_create();
  row->basePrice = 3;
  DocumentRowList_pushBack(&list, row);

  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 0)->basePrice, 1);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 1)->basePrice, 2);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 2)->basePrice, 3);
  ASSERT_EQUAL(DocumentRowList_get(list, 10), NULL);
  ASSERT_EQUAL(DocumentRowList_getRowCount(list), 3);

  row = DocumentRow_create();
  row->basePrice = -1;
  DocumentRowList_insertBefore(&list, DocumentRowList_get(list, 0), row);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 0)->basePrice, -1);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 1)->basePrice, 1);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 2)->basePrice, 2);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 3)->basePrice, 3);
  ASSERT_EQUAL(DocumentRowList_getRowCount(list), 4);

  row = DocumentRow_create();
  row->basePrice = 1.5;
  DocumentRowList_insertBefore(&list, DocumentRowList_get(list, 2), row);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 0)->basePrice, -1);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 1)->basePrice, 1);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 2)->basePrice, 1.5);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 3)->basePrice, 2);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 4)->basePrice, 3);
  ASSERT_EQUAL(DocumentRowList_getRowCount(list), 5);

  row = DocumentRow_create();
  row->basePrice = 2.5;
  DocumentRowList_insertAfter(&list, DocumentRowList_get(list, 3), row);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 0)->basePrice, -1);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 1)->basePrice, 1);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 2)->basePrice, 1.5);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 3)->basePrice, 2);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 4)->basePrice, 2.5);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 5)->basePrice, 3);
  ASSERT_EQUAL(DocumentRowList_getRowCount(list), 6);

  row = DocumentRow_create();
  row->basePrice = 3.5;
  DocumentRowList_insertAfter(&list, DocumentRowList_get(list, 5), row);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 0)->basePrice, -1);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 1)->basePrice, 1);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 2)->basePrice, 1.5);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 3)->basePrice, 2);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 4)->basePrice, 2.5);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 5)->basePrice, 3);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 6)->basePrice, 3.5);
  ASSERT_EQUAL(DocumentRowList_getRowCount(list), 7);

  DocumentRowList_removeRow(&list, DocumentRowList_get(list, 6));
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 0)->basePrice, -1);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 1)->basePrice, 1);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 2)->basePrice, 1.5);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 3)->basePrice, 2);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 4)->basePrice, 2.5);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 5)->basePrice, 3);
  ASSERT_EQUAL(DocumentRowList_getRowCount(list), 6);

  DocumentRowList_removeRow(&list, DocumentRowList_get(list, 3));
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 0)->basePrice, -1);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 1)->basePrice, 1);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 2)->basePrice, 1.5);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 3)->basePrice, 2.5);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 4)->basePrice, 3);
  ASSERT_EQUAL(DocumentRowList_getRowCount(list), 5);

  DocumentRowList_removeRow(&list, DocumentRowList_get(list, 0));
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 0)->basePrice, 1);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 1)->basePrice, 1.5);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 2)->basePrice, 2.5);
  ASSERT_EQUAL_DOUBLE(DocumentRowList_get(list, 3)->basePrice, 3);
  ASSERT_EQUAL(DocumentRowList_getRowCount(list), 4);

  DocumentRowList_finalize(&list);
}

void test_DocumentRowList(void)
{
  BEGIN_TESTS(DocumentRowList)
  {
    RUN_TEST(test_DocumentRowList_init);
    RUN_TEST(test_DocumentRowList_create);
    RUN_TEST(test_DocumentRowList_readAndWriteRow);
    RUN_TEST(test_DocumentRowList_generic);
  }
  END_TESTS
}
