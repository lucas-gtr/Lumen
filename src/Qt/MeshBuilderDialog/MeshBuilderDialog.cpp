#include "Qt/MeshBuilderDialog/MeshBuilderDialog.hpp"

#include <qboxlayout.h>
#include <qpushbutton.h>

// NOLINTBEGIN(cppcoreguidelines-owning-memory)
void MeshBuilderDialog::addButtonsRow() {
  auto* ok_button     = new QPushButton("OK", this);
  auto* cancel_button = new QPushButton("Cancel", this);
  auto* button_layout = new QHBoxLayout;
  button_layout->addWidget(ok_button);
  button_layout->addWidget(cancel_button);
  m_layout->addRow(button_layout);

  connect(ok_button, &QPushButton::clicked, this, &MeshBuilderDialog::accept);
  connect(cancel_button, &QPushButton::clicked, this, &MeshBuilderDialog::reject);
}
// NOLINTEND(cppcoreguidelines-owning-memory)