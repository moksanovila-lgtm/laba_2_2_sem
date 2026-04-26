#include <QApplication>
#include <QStyleFactory>
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    // Создаём Qt приложение
    QApplication app(argc, argv);
    
    // Настройки приложения
    app.setApplicationName("Sequence System");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("University");
    
    // Устанавливаем стиль (опционально, для более современного вида)
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Создаём и показываем главное окно
    MainWindow window;
    window.setWindowTitle("Sequence System - Laboratory Work #2");
    window.setMinimumSize(900, 700);
    window.show();
    
    // Запускаем цикл обработки событий
    return app.exec();
}