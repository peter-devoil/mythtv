#ifndef PLAYLISTEDITORVIEW_H_
#define PLAYLISTEDITORVIEW_H_

// qt
#include <QEvent>
#include <QVector>

// mythui
#include <mythscreentype.h>
#include <mythgenerictree.h>
#include <mythuibuttonlist.h>

// mythmusic
#include "musiccommon.h"

class MythUIButtonTree;
class MythUIText;
class MythMenu;

// This is just so we can use the guarded pointers provided by QPointer in the MusicGenericTree
class MPUBLIC MusicButtonItem : public MythUIButtonListItem, public QObject
{
  public:
    MusicButtonItem(MythUIButtonList *lbtype, const QString& text,
                         const QString& image = "", bool checkable = false,
                         CheckState state = CantCheck, bool showArrow = false,
                         int listPosition = -1):
        MythUIButtonListItem(lbtype, text, image, checkable, state, showArrow, listPosition) {}

    MusicButtonItem(MythUIButtonList *lbtype, const QString& text, QVariant data, int listPosition = -1) :
        MythUIButtonListItem(lbtype, text, data, listPosition) {}
};

class MPUBLIC MusicGenericTree : public MythGenericTree
{
  public:
    MusicGenericTree(MusicGenericTree *parent, const QString &name,
                     const QString &action = "",
                     MythUIButtonListItem::CheckState check = MythUIButtonListItem::CantCheck,
                     bool showArrow = true);
    virtual ~MusicGenericTree() = default;

    QString getAction(void) const { return m_action; }

    MythUIButtonListItem::CheckState getCheck(void) const { return m_check; }
    void setCheck(MythUIButtonListItem::CheckState state);

    void setDrawArrow(bool flag);

    MythUIButtonListItem *CreateListButton(MythUIButtonList *list) override; // MythGenericTree

  protected:
    QString  m_action;
    QPointer<MusicButtonItem> m_buttonItem {nullptr};
    MythUIButtonListItem::CheckState m_check {MythUIButtonListItem::CantCheck};
    bool     m_showArrow {true};
};

Q_DECLARE_METATYPE(MusicGenericTree*);

class PlaylistEditorView : public MusicCommon
{
    Q_OBJECT
  public:
    PlaylistEditorView(MythScreenStack *parent, MythScreenType *parentScreen,
                       const QString &layout, bool restorePosition = false);
    ~PlaylistEditorView(void);

    bool Create(void) override; // MythScreenType
    bool keyPressEvent(QKeyEvent *) override; // MusicCommon

    void saveTreePosition(void);

    void ShowMenu(void) override; // MusicCommon

  protected:
    void customEvent(QEvent *event) override; // MusicCommon

  private slots:
    void treeItemClicked(MythUIButtonListItem *item);
    void treeItemVisible(MythUIButtonListItem *item);
    void treeNodeChanged(MythGenericTree *node);
    void smartPLChanged(const QString &category, const QString &name);
    void deleteSmartPlaylist(bool ok);
    void deletePlaylist(bool ok);

  private:
    void filterTracks(MusicGenericTree *node);

    void getPlaylists(MusicGenericTree *node);
    void getPlaylistTracks(MusicGenericTree *node, int playlistID);

    void getSmartPlaylistCategories(MusicGenericTree *node);
    void getSmartPlaylists(MusicGenericTree *node);
    void getSmartPlaylistTracks(MusicGenericTree *node, int playlistID);

    void getCDTracks(MusicGenericTree *node);

    void updateSelectedTracks(void);
    void updateSelectedTracks(MusicGenericTree *node);
    void updateSonglist(MusicGenericTree *node);

    void createRootNode(void);
    void reloadTree(void);
    void restoreTreePosition(const QStringList &route);

    MythMenu* createPlaylistMenu(void);
    MythMenu* createSmartPlaylistMenu(void);

  private:
    QString                 m_layout;
    bool                    m_restorePosition {false};
    MusicGenericTree       *m_rootNode        {nullptr};
    QList<MetadataPtrList*> m_deleteList;

    MythUIButtonTree *m_playlistTree          {nullptr};
    MythUIText       *m_breadcrumbsText       {nullptr};
    MythUIText       *m_positionText          {nullptr};
};

#endif
