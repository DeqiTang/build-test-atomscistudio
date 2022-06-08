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

#include "occwindow.h"

class OccView : public QWidget, protected AIS_ViewController {
    Q_OBJECT
public:

    OccView(QWidget* parent = nullptr);
    ~OccView();

    Standard_EXPORT static void onButtonUserAction(int exercise_step, Handle(AIS_InteractiveContext)&);
    Standard_EXPORT static void doSelection(int id, Handle(AIS_InteractiveContext)&);

    const Handle(V3d_View)& get_view() const {
        return m_v3d_view;
    }
    const Handle(AIS_InteractiveContext)& get_context() const {
        return m_context;
    }

public:
    enum CursorAction {
        Nothing,
        DynamicZooming,
        WindowZooming,
        DynamicPanning,
        GlobalPanning,
        DynamicRotation,
        Selecting
    };

    enum DrawStyle {
        WireFrame,
        Shaded,
    };
signals:

    void selection_changed();

public slots:

    void orbit() {
        set_mouse_gestures(CursorAction::Nothing);
    }
    void select() {
        set_mouse_gestures(CursorAction::Selecting);
    }
    void zoom() {
        set_mouse_gestures(CursorAction::DynamicZooming);
    }
    void pan() {
        set_mouse_gestures(CursorAction::DynamicPanning);
    }

    void fit_all() {
        m_v3d_view->FitAll();
        m_v3d_view->ZFitAll();
        m_v3d_view->Redraw();
    }

protected:
    virtual QPaintEngine* paintEngine() const override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

    void set_mouse_gestures(CursorAction mode);

    void OnSelectionChanged(
        const Handle(AIS_InteractiveContext)& context,
        const Handle(V3d_View)& view
    ) Standard_OVERRIDE;

private:

    bool m_is_raytracing{false};

    Handle(V3d_Viewer) m_v3d_viewer;
    Handle(V3d_View) m_v3d_view;
    Handle(AIS_InteractiveContext) m_context;

    AIS_MouseGestureMap m_mouse_default_gestures;
    Graphic3d_Vec2i m_click_pos;
    CursorAction m_cur_mode;
    DrawStyle m_draw_style;
    Standard_Real m_cur_zoom{0};
    const Standard_Real m_device_px;

    Handle(Aspect_DisplayConnection) m_display_connection;
    Handle(OpenGl_GraphicDriver) m_graphic_driver;
    OccWindow* m_occwindow;
};

#endif // MODELING_OCCVIEW_H
