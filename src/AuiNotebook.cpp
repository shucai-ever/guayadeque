// -------------------------------------------------------------------------------- //
//	Copyright (C) 2008-2009 J.Rios
//	anonbeat@gmail.com
//
//    This Program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2, or (at your option)
//    any later version.
//
//    This Program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; see the file LICENSE.  If not, write to
//    the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
//    http://www.gnu.org/copyleft/gpl.html
//
// -------------------------------------------------------------------------------- //
#include "AuiNotebook.h"

#include "Utils.h"

#include <wx/dc.h>

// -------------------------------------------------------------------------------- //
static void inline IndentPressedBitmap( wxRect * rect, int button_state )
{
    if (button_state == wxAUI_BUTTON_STATE_PRESSED)
    {
        rect->x++;
        rect->y++;
    }
}

// -------------------------------------------------------------------------------- //
// guAuiTabArt
// -------------------------------------------------------------------------------- //
guAuiTabArt::guAuiTabArt() : wxAuiDefaultTabArt()
{
    m_BgColor = wxSystemSettings::GetColour( wxSYS_COLOUR_3DHILIGHT );
    m_SelBgColor = wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT );
    //m_TextFgColor.Set( m_base_colour.Red() ^ 0xFF, m_base_colour.Green() ^ 0xFF, m_base_colour.Blue() ^ 0xFF );
    m_TextFgColor = wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVECAPTIONTEXT );
    m_SelTextFgColour = wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT );
    //m_TextFgColor = * wxBLUE;
//    guLogMessage( wxT( "FGColor:%s" ), m_TextFgColor.GetAsString( wxC2S_HTML_SYNTAX ).c_str() );
}

// -------------------------------------------------------------------------------- //
guAuiTabArt::~guAuiTabArt()
{
}

// -------------------------------------------------------------------------------- //
wxAuiTabArt * guAuiTabArt::Clone()
{
    guAuiTabArt * art = new guAuiTabArt;
    art->SetNormalFont( m_normal_font );
    art->SetSelectedFont( m_selected_font );
    art->SetMeasuringFont( m_measuring_font );
//    art->m_BgColor = m_BgColor;
//    art->m_SelBgColor = m_SelBgColor;
//    art->m_TextFgColor = m_TextFgColor;
    return art;
}

// -------------------------------------------------------------------------------- //
void guAuiTabArt::DrawTab(wxDC &dc, wxWindow * wnd, const wxAuiNotebookPage &page,
        const wxRect & in_rect, int close_button_state, wxRect * out_tab_rect,
        wxRect * out_button_rect, int * x_extent )
{
    wxCoord normal_textx, normal_texty;
    wxCoord selected_textx, selected_texty;
    wxCoord texty;

    // if the caption is empty, measure some temporary text
    wxString caption = page.caption;
    if (caption.empty())
        caption = wxT("Xj");

    dc.SetFont(m_selected_font);
    dc.GetTextExtent(caption, &selected_textx, &selected_texty);

    dc.SetFont(m_normal_font);
    dc.GetTextExtent(caption, &normal_textx, &normal_texty);

    // figure out the size of the tab
    wxSize tab_size = GetTabSize(dc,
                                 wnd,
                                 page.caption,
                                 page.bitmap,
                                 page.active,
                                 close_button_state,
                                 x_extent);

    wxCoord tab_height = m_tab_ctrl_height - 3;
    wxCoord tab_width = tab_size.x;
    wxCoord tab_x = in_rect.x;
    wxCoord tab_y = in_rect.y + in_rect.height - tab_height;


    caption = page.caption;


    // select pen, brush and font for the tab to be drawn

    if (page.active)
    {
        dc.SetFont(m_selected_font);
        texty = selected_texty;
    }
     else
    {
        dc.SetFont(m_normal_font);
        texty = normal_texty;
    }


    // create points that will make the tab outline

    int clip_width = tab_width;
    if (tab_x + clip_width > in_rect.x + in_rect.width)
        clip_width = (in_rect.x + in_rect.width) - tab_x;

/*
    wxPoint clip_points[6];
    clip_points[0] = wxPoint(tab_x,              tab_y+tab_height-3);
    clip_points[1] = wxPoint(tab_x,              tab_y+2);
    clip_points[2] = wxPoint(tab_x+2,            tab_y);
    clip_points[3] = wxPoint(tab_x+clip_width-1, tab_y);
    clip_points[4] = wxPoint(tab_x+clip_width+1, tab_y+2);
    clip_points[5] = wxPoint(tab_x+clip_width+1, tab_y+tab_height-3);

    // FIXME: these ports don't provide wxRegion ctor from array of points
#if !defined(__WXDFB__) && !defined(__WXCOCOA__)
    // set the clipping region for the tab --
    wxRegion clipping_region(WXSIZEOF(clip_points), clip_points);
    dc.SetClippingRegion(clipping_region);
#endif // !wxDFB && !wxCocoa
*/
    // since the above code above doesn't play well with WXDFB or WXCOCOA,
    // we'll just use a rectangle for the clipping region for now --
    dc.SetClippingRegion(tab_x, tab_y, clip_width+1, tab_height-3);


    wxPoint border_points[6];
    if (m_flags &wxAUI_NB_BOTTOM)
    {
       border_points[0] = wxPoint(tab_x,             tab_y);
       border_points[1] = wxPoint(tab_x,             tab_y+tab_height-6);
       border_points[2] = wxPoint(tab_x+2,           tab_y+tab_height-4);
       border_points[3] = wxPoint(tab_x+tab_width-2, tab_y+tab_height-4);
       border_points[4] = wxPoint(tab_x+tab_width,   tab_y+tab_height-6);
       border_points[5] = wxPoint(tab_x+tab_width,   tab_y);
    }
    else //if (m_flags & wxAUI_NB_TOP) {}
    {
       border_points[0] = wxPoint(tab_x,             tab_y+tab_height-4);
       border_points[1] = wxPoint(tab_x,             tab_y+2);
       border_points[2] = wxPoint(tab_x+2,           tab_y);
       border_points[3] = wxPoint(tab_x+tab_width-2, tab_y);
       border_points[4] = wxPoint(tab_x+tab_width,   tab_y+2);
       border_points[5] = wxPoint(tab_x+tab_width,   tab_y+tab_height-4);
    }
    // TODO: else if (m_flags &wxAUI_NB_LEFT) {}
    // TODO: else if (m_flags &wxAUI_NB_RIGHT) {}

    int drawn_tab_yoff = border_points[1].y;
    int drawn_tab_height = border_points[0].y - border_points[1].y;


    if (page.active)
    {
        wxRect r(tab_x, tab_y+1, tab_width, tab_height-3);

        // start the gradent up a bit and leave the inside border inset
        // by a pixel for a 3D look.  Only the top half of the inactive
        // tab will have a slight gradient
        r.x += 3;
        r.y++;
        r.width -= 4;
        r.height /= 2;
        r.height--;

        // -- draw top gradient fill for glossy look
        wxColor top_color = m_SelBgColor;
        wxColor bottom_color = wxAuiStepColour(m_base_colour, 140);
        dc.GradientFillLinear(r, bottom_color, top_color, wxNORTH);

        r.y += r.height;
        r.y--;

        // -- draw bottom fill for glossy look
        top_color = m_base_colour;
        bottom_color = m_base_colour;
        dc.GradientFillLinear(r, top_color, bottom_color, wxSOUTH);
    }
     else
    {
        // draw inactive tab

        wxRect r(tab_x, tab_y+1, tab_width, tab_height-3);

        // start the gradent up a bit and leave the inside border inset
        // by a pixel for a 3D look.  Only the top half of the inactive
        // tab will have a slight gradient
        r.x += 3;
        r.y++;
        r.width -= 4;
        r.height /= 2;
        r.height--;

        // -- draw top gradient fill for glossy look
        wxColor top_color = m_base_colour;
        wxColor bottom_color = wxAuiStepColour(top_color, 160);
        dc.GradientFillLinear(r, bottom_color, top_color, wxNORTH);

        r.y += r.height;
        r.y--;

        // -- draw bottom fill for glossy look
        top_color = m_base_colour;
        bottom_color = m_base_colour;
        dc.GradientFillLinear(r, top_color, bottom_color, wxSOUTH);
    }

    // draw tab outline
    dc.SetPen(m_border_pen);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawPolygon(WXSIZEOF(border_points), border_points);

    // there are two horizontal grey lines at the bottom of the tab control,
    // this gets rid of the top one of those lines in the tab control
    if (page.active)
    {
       if (m_flags &wxAUI_NB_BOTTOM)
           dc.SetPen(wxPen(wxColour(wxAuiStepColour(m_base_colour, 170))));
       // TODO: else if (m_flags &wxAUI_NB_LEFT) {}
       // TODO: else if (m_flags &wxAUI_NB_RIGHT) {}
       else //for wxAUI_NB_TOP
           dc.SetPen(m_base_colour_pen);
        dc.DrawLine(border_points[0].x+1,
                    border_points[0].y,
                    border_points[5].x,
                    border_points[5].y);
    }


    int text_offset = tab_x + 8;
    int close_button_width = 0;
    if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN)
    {
        close_button_width = m_active_close_bmp.GetWidth();
    }


    int bitmap_offset = 0;
    if (page.bitmap.IsOk())
    {
        bitmap_offset = tab_x + 8;

        // draw bitmap
        dc.DrawBitmap(page.bitmap,
                      bitmap_offset,
                      drawn_tab_yoff + (drawn_tab_height/2) - (page.bitmap.GetHeight()/2),
                      true);

        text_offset = bitmap_offset + page.bitmap.GetWidth();
        text_offset += 3; // bitmap padding
    }
     else
    {
        text_offset = tab_x + 8;
    }


    wxString draw_text = wxAuiChopText(dc,
                          caption,
                          tab_width - (text_offset-tab_x) - close_button_width);

    dc.SetTextForeground( page.active ? m_SelTextFgColour : m_TextFgColor );

    // draw tab text
    dc.DrawText(draw_text,
                text_offset,
                drawn_tab_yoff + (drawn_tab_height)/2 - (texty/2) - 1);

    // draw close button if necessary
    if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN)
    {
        wxBitmap bmp = m_disabled_close_bmp;

        if (close_button_state == wxAUI_BUTTON_STATE_HOVER ||
            close_button_state == wxAUI_BUTTON_STATE_PRESSED)
        {
            bmp = m_active_close_bmp;
        }

        wxRect rect(tab_x + tab_width - close_button_width - 1,
                    tab_y + (tab_height/2) - (bmp.GetHeight()/2),
                    close_button_width,
                    tab_height);
        IndentPressedBitmap(&rect, close_button_state);
        dc.DrawBitmap(bmp, rect.x, rect.y, true);

        *out_button_rect = rect;
    }

    *out_tab_rect = wxRect(tab_x, tab_y, tab_width, tab_height);

    dc.DestroyClippingRegion();
}

// -------------------------------------------------------------------------------- //
// guAuiNotebook
// -------------------------------------------------------------------------------- //
guAuiNotebook::guAuiNotebook() : wxAuiNotebook()
{
    SetArtProvider( new guAuiTabArt() );
}

// -------------------------------------------------------------------------------- //
guAuiNotebook::guAuiNotebook(wxWindow *parent,
                             wxWindowID id,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style) :
    wxAuiNotebook( parent, id, pos, size, style )
{
    SetArtProvider( new guAuiTabArt() );
}

// -------------------------------------------------------------------------------- //
guAuiNotebook::~guAuiNotebook()
{
}

// UpdateTabCtrlHeight() does the actual tab resizing. It's meant
// to be used interally
// -------------------------------------------------------------------------------- //
void guAuiNotebook::UpdateTabCtrlHeight()
{
    // get the tab ctrl height we will use
    int height = CalculateTabCtrlHeight();

    wxAuiTabArt* art = m_tabs.GetArtProvider();

    m_tab_ctrl_height = height;

    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    size_t i, pane_count = all_panes.GetCount();
    for (i = 0; i < pane_count; ++i)
    {
        wxAuiPaneInfo& pane = all_panes.Item(i);
        if (pane.name == wxT("dummy"))
            continue;
        wxTabFrame* tab_frame = (wxTabFrame*)pane.window;
        wxAuiTabCtrl* tabctrl = tab_frame->m_tabs;
        tab_frame->SetTabCtrlHeight(m_tab_ctrl_height);
        tabctrl->SetArtProvider(art->Clone());
        tab_frame->DoSizing();
    }
}

// -------------------------------------------------------------------------------- //
wxString guAuiNotebook::SavePerspective( void )
{
    // Build list of panes/tabs
    wxString tabs;
    wxAuiPaneInfoArray &all_panes = m_mgr.GetAllPanes();
    const size_t pane_count = all_panes.GetCount();

    for( size_t i = 0; i < pane_count; ++i )
    {
        wxAuiPaneInfo &pane = all_panes.Item( i );
        if( pane.name == wxT( "dummy" ) )
            continue;

        wxTabFrame * tabframe = ( wxTabFrame * ) pane.window;

        if( !tabs.empty() )
            tabs += wxT( "|" );
        tabs += pane.name;
        tabs += wxT( "=" );

        // add tab id's
        size_t page_count = tabframe->m_tabs->GetPageCount();
        for( size_t p = 0; p < page_count; ++p )
        {
            wxAuiNotebookPage &page = tabframe->m_tabs->GetPage( p );
            const size_t page_idx = m_tabs.GetIdxFromWindow( page.window );

            if( p )
                tabs += wxT( "," );

            if( ( int ) page_idx == m_curpage )
                tabs += wxT( "*" );
            else if( ( int ) p == tabframe->m_tabs->GetActivePage() )
                tabs += wxT( "+" );
            tabs += wxString::Format( wxT( "%02u[%s]" ), page_idx, page.caption.c_str() );
        }
    }
    tabs += wxT( "@" );

    // Add frame perspective
    tabs += m_mgr.SavePerspective();

    return tabs;
}

// -------------------------------------------------------------------------------- //
bool guAuiNotebook::LoadPerspective( const wxString &layout )
{
    // Remove all tab ctrls (but still keep them in main index)
    const size_t tab_count = m_tabs.GetPageCount();
    for( size_t i = 0; i < tab_count; ++i )
    {
        wxWindow * wnd = m_tabs.GetWindowFromIdx( i );

        // find out which onscreen tab ctrl owns this tab
        wxAuiTabCtrl * ctrl;
        int ctrl_idx;
        if( !FindTab( wnd, &ctrl, &ctrl_idx ) )
            return false;

        // remove the tab from ctrl
        if( !ctrl->RemovePage( wnd ) )
            return false;
    }
    RemoveEmptyTabFrames();

    size_t sel_page = 0;

    wxString tabs = layout.BeforeFirst( wxT( '@' ) );
    while( 1 )
    {
        const wxString tab_part = tabs.BeforeFirst( wxT( '|' ) );

        // if the string is empty, we're done parsing
        if( tab_part.empty() )
            break;

        // Get pane name
        const wxString pane_name = tab_part.BeforeFirst( wxT( '=' ) );

        // create a new tab frame
        wxTabFrame * new_tabs = new wxTabFrame;
        new_tabs->m_tabs = new wxAuiTabCtrl( this,
                                    m_tab_id_counter++,
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    wxNO_BORDER | wxWANTS_CHARS );
        new_tabs->m_tabs->SetArtProvider( m_tabs.GetArtProvider()->Clone() );
        new_tabs->SetTabCtrlHeight( m_tab_ctrl_height );
        new_tabs->m_tabs->SetFlags( m_flags );
        wxAuiTabCtrl * dest_tabs = new_tabs->m_tabs;

        // create a pane info structure with the information
        // about where the pane should be added
        wxAuiPaneInfo pane_info = wxAuiPaneInfo().Name( pane_name ).Bottom().CaptionVisible( false );
        m_mgr.AddPane( new_tabs, pane_info );

        // Get list of tab id's and move them to pane
        wxString tab_list = tab_part.AfterFirst( wxT( '=' ) );
        while( 1 )
        {
            wxString tab = tab_list.BeforeFirst( wxT( ',' ) );
            if( tab.empty() )
                break;
            tab_list = tab_list.AfterFirst( wxT( ',' ) );

            // Check if this page has an 'active' marker
            const wxChar c = tab[ 0 ];
            if( c == wxT( '+' ) || c == wxT( '*' ) )
            {
                tab = tab.Mid( 1, 2 );
            }

            const size_t tab_idx = wxAtoi( tab.c_str() );
            if( tab_idx >= GetPageCount() )
                continue;

            // Move tab to pane
            wxAuiNotebookPage &page = m_tabs.GetPage( tab_idx );
            const size_t newpage_idx = dest_tabs->GetPageCount();
            dest_tabs->InsertPage( page.window, page, newpage_idx );

            if( c == wxT( '+' ) )
                dest_tabs->SetActivePage( newpage_idx );
            else if( c == wxT( '*' ) )
                sel_page = tab_idx;
        }
        dest_tabs->DoShowHide();

        tabs = tabs.AfterFirst( wxT( '|' ) );
    }

    // Load the frame perspective
    const wxString frames = layout.AfterFirst( wxT( '@' ) );
    m_mgr.LoadPerspective( frames );

    // Force refresh of selection
    m_curpage = -1;
    SetSelection( sel_page );

    return true;
}

// -------------------------------------------------------------------------------- //
wxAuiPaneInfo & guAuiNotebook::GetPane( wxWindow * window )
{
    return m_mgr.GetPane( window );
}

// -------------------------------------------------------------------------------- //
wxAuiPaneInfo & guAuiNotebook::GetPane( const wxString &name )
{
    return m_mgr.GetPane( name );
}

// -------------------------------------------------------------------------------- //
wxAuiPaneInfoArray & guAuiNotebook::GetAllPanes( void )
{
    return m_mgr.GetAllPanes();
}

// -------------------------------------------------------------------------------- //