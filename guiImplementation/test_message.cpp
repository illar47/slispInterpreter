#include <QDebug>
#include <QtTest/QtTest>
#include <QtWidgets>

#include "message_widget.hpp"

class TestMessage : public QObject {
  Q_OBJECT

public:
private slots:

  void initTestCase();
  void testConstructor();
  
private:

  MessageWidget message;

  QLineEdit * messageEdit;
};

void TestMessage::initTestCase() {

  messageEdit = message.findChild<QLineEdit *>();
  QVERIFY2(messageEdit,
           "Could not find QLineEdit instance in MessageWidget instance.");

}

void TestMessage::testConstructor() {

  QVERIFY(messageEdit);

  // check message
  QVERIFY2(messageEdit->isReadOnly(),
           "Expected QLineEdit inside MessageWidget to be read-only.");
  QCOMPARE(messageEdit->text(), QString(""));
}


QTEST_MAIN(TestMessage)
#include "test_message.moc"
