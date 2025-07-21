/**
 * @file FileSelector.hpp
 * @brief Header file for the FileSelector class, which provides a widget to select files or directories.
 */
#ifndef GUI_WIDGETS_FILESELECTOR_HPP
#define GUI_WIDGETS_FILESELECTOR_HPP

#include <QString>
#include <QWidget>
#include <cstdint>

namespace Ui {
class FileSelector;
}

/**
 * @enum FileSelectorMode
 * @brief Enumeration for the mode of the file selector.
 *
 * This enum defines the modes for the file selector, including folder selection and image file selection.
 */
enum class FileSelectorMode : std::uint8_t { FOLDER, IMAGE_FILE };

/**
 * @class FileSelector
 * @brief A widget that allows users to select files or directories.
 *
 * This class provides a user interface for selecting files or directories, with different modes for folder and image
 * file selection. It emits a signal when the selected path changes.
 */
class FileSelector : public QWidget {
  Q_OBJECT

public:
  /**
   * @brief Constructor for the FileSelector class.
   * @param parent The parent widget for this file selector.
   */
  explicit FileSelector(QWidget* parent = nullptr);

  /**
   * @brief Sets the mode of the file selector.
   * @param mode The mode to set for the file selector (folder or image file).
   */
  void setMode(FileSelectorMode mode);

  /**
   * @brief Sets the path of the file selector.
   * @param path The new path to set for the file selector.
   */
  void setPath(const QString& path);

  /**
   * @brief Gets the current path of the file selector.
   * @return The current path as a QString.
   */
  QString getPath() const;

  ~FileSelector(); ///< Default destructor.

signals:
  void pathChanged(const QString& newPath);

private:
  Ui::FileSelector* ui;
  FileSelectorMode  m_mode = FileSelectorMode::FOLDER;

  void setupConnections();
  void openDialog();
};

#endif // GUI_WIDGETS_FILESELECTOR_HPP
