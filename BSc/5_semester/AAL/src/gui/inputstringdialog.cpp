#include "inc/gui/inputstringdialog.h"
#include "inc/gui/aalbackend.h"
#include "inc/gui/inputstringvalidator.h"
#include "ui_inputstringdialog.h"

InputStringDialog::InputStringDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputStringDialog) {
    ui->setupUi(this);
    ui->input->setValidator(new InputStringValidator());
    connect(ui->generate_button, &QPushButton::clicked, this, &InputStringDialog::on_generate_button_clicked);
    connect(ui->generator_selector, qOverload<int>(&QComboBox::currentIndexChanged), this, &InputStringDialog::generator_type_changed);
}

InputStringDialog::~InputStringDialog() {
    delete ui;
}

void InputStringDialog::on_generate_button_clicked() {
    std::string generated;
    const auto len = ui->input_length->value();
    switch (ui->generator_selector->currentIndex()) {
        case 0: 
            generated = AALBackend::generateRandom(len); 
            break;

        case 1: 
            generated = AALBackend::generateRandomPermutation( 
                    RandomPermutationParams {
                       static_cast<std::size_t>(ui->C_amount->value()),
                       static_cast<std::size_t>(ui->M_amount->value()),
                       static_cast<std::size_t>(ui->Y_amount->value()),
                       static_cast<std::size_t>(ui->K_amount->value())
                    }
                ); 
            break;

        case 2: 
            generated = AALBackend::generateRandomWithChance(len, ui->correct_chance->value()); 
            break;
    }

    const auto generated_input = QString(generated.c_str()); 
    ui->input->setText(generated_input);
}

QString InputStringDialog::input() const noexcept {
    return ui->input->text();
}
    
void InputStringDialog::generator_type_changed(int index) {
    if (index == 1)
        ui->input_length->setEnabled(false);
    else
        ui->input_length->setEnabled(true);
}
