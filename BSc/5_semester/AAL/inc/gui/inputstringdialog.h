#ifndef INPUTSTRINGDIALOG_H
#define INPUTSTRINGDIALOG_H

#include <QDialog>

namespace Ui {
class InputStringDialog;
}

class InputStringDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QString input READ input)

public:
    explicit InputStringDialog(QWidget *parent = nullptr);
    ~InputStringDialog();

    QString input() const noexcept;

private:
    void on_generate_button_clicked();
    void generator_type_changed(int index);
    Ui::InputStringDialog *ui;
};

#endif // INPUTSTRINGDIALOG_H
