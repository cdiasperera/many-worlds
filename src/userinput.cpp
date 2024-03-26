#include <QDebug>

#include "mainview.h"

/**
 * @brief MainView::keyPressEvent Triggered by pressing a key.
 * @param ev Key event.
 */
void MainView::keyPressEvent(QKeyEvent *ev) {
    keyboardStatus.updateStatus(ev->key(), KeyboardStatus::KEY_STATUS::DOWN);

    update();
}

/**
 * @brief MainView::keyReleaseEvent Triggered by releasing a key.
 * @param ev Key event.
 */
void MainView::keyReleaseEvent(QKeyEvent *ev) {
    keyboardStatus.updateStatus(ev->key(), KeyboardStatus::KEY_STATUS::UP);

    update();
}

/**
 * @brief MainView::mousePressEvent Triggered when pressing any mouse button.
 * @param ev Mouse event.
 */
void MainView::mousePressEvent(QMouseEvent *ev) {
  qDebug() << "Mouse button pressed:" << ev->button();

  update();
  // Do not remove the line below, clicking must focus on this widget!
  this->setFocus();
}
