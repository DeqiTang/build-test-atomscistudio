#ifndef MODELING_OCCVIEW_H
#define MODELING_OCCVIEW_H

#include <QWidget>
#include <QMenu>
#include <QFileDialog>
#include <QMouseEvent>

#include <AIS_InteractiveContext.hxx>
#include <AIS_ViewController.hxx>
#include <V3d_View.hxx>

#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>

class OccView : public QWidget, protected AIS_ViewController {
    Q_OBJECT
public:

    OccView(QWidget* parent = nullptr);
    ~OccView();

    const Handle(V3d_View)& get_view() const {
        return m_v3d_view;
    }
    const Handle(AIS_InteractiveContext)& get_context() const {
        return m_context;
    }

signals:

    void selection_changed();

public slots:

    void fit_view() {
        FitAllAuto(m_context, m_v3d_view);
    }

protected:
    virtual QPaintEngine* paintEngine() const override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

    void OnSelectionChanged(
        const Handle(AIS_InteractiveContext)& context,
        const Handle(V3d_View)& view
    ) Standard_OVERRIDE;

private:

    Handle(V3d_Viewer) m_v3d_viewer;
    Handle(V3d_View) m_v3d_view;
    Handle(AIS_InteractiveContext) m_context;

    Graphic3d_Vec2i m_click_pos;
    AIS_DisplayMode m_draw_style;
    Standard_Real m_cur_zoom{0};
    const Standard_Real m_device_px;

    Handle(Aspect_DisplayConnection) m_display_connection;
    Handle(Graphic3d_GraphicDriver) m_graphic_driver;
    // Handle(OpenGl_GraphicDriver) m_graphic_driver;
    Handle(Aspect_Window) m_occwindow;
};

#endif // MODELING_OCCVIEW_H
