/*
  This file is part of the kblog library.

  SPDX-FileCopyrightText: 2007 Mike McQuaid <mike@mikemcquaid.com>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KBLOG_LIVEJOURNAL_H
#define KBLOG_LIVEJOURNAL_H

#include <blog.h>

#include <QColor>

class QUrl;

/**
  @file
  This file is part of the  for accessing Blog Servers
  and defines the LiveJournal class.

  @author Mike McQuaid \<mike\@mikemcquaid.com\>
*/
namespace KBlog
{

class LiveJournalPrivate;

/**
  @brief
  A class that can be used for access to Movable Type  blogs. Almost every
  blog server supports Movable Type . Blah blah
  @code
  Blog* myblog = new LiveJournal("http://example.com/xmlrpc/gateway.php");
  KBlog::BlogPost *post = new BlogPost();
  post->setUserId( "some_user_id" );
  post->setTitle( "This is the title." );
  post->setContent( "Here is some the content..." );
  myblog->createPost( post );
  @endcode

  @author Mike McQuaid \<mike\@mikemcquaid.com\>
*/
class KBLOG_EXPORT LiveJournal : public Blog
{
    Q_OBJECT
public:
    /**
      Create an object for Movable Type

      @param server is the url for the xmlrpc gateway.
      @param parent is the parent object.
    */
    explicit LiveJournal(const QUrl &server, QObject *parent = nullptr);

    /**
      Destroy the object.
    */
    virtual ~LiveJournal();

    virtual void addFriend(const QString &username, int group,
                           const QColor &fgcolor = QColor(0, 0, 0),
                           const QColor &bgcolor = QColor(255, 255, 255));

    virtual void assignFriendToCategory(const QString &username, int category);

    /**
      Create a new post on server.

      @param post is send to the server.
    */
    void createPost(KBlog::BlogPost *post);

    virtual void deleteFriend(const QString &username);

    /**
      Fetch the Post with postId.
      @param postId is the id of the post on the server.

      @see  void fetchedPost( KBlog::BlogPost &post )
    */
    void fetchPost(KBlog::BlogPost *post);

    virtual void fetchUserInfo();

    QString fullName() const;

    /**
      Returns the  of the inherited object.
    */
    QString interfaceName() const;

    void listCategories();

    virtual void listFriends();

    virtual void listFriendsOf();

    virtual void listMoods();

    virtual void listPictureKeywords();

    /**
      List recent posts on the server.
      @see     void listedPost( KBlog::BlogPost &post )

      @see     void listRecentPostsFinished()
    */
    void listRecentPosts(int number);

    /**
      Modify a post on server.

      @param post is used to send the modified post including the
      correct postId from it to the server.
    */
    void modifyPost(KBlog::BlogPost *post);

    void removePost(KBlog::BlogPost *post);
    /**
      Set the Url of the server.

      @param server is the server url.
    */
    void setUrl(const QUrl &server);

    QString serverMessage() const;

    QString userId() const;

Q_SIGNALS:
    void addedFriend();
    void assignedFriendToCategory();
    void deletedFriend();
    void expiredCookie();
    void expiredAllCookies();
    void generatedCookie(const QString &cookie);
    void listedCategories(const QMap<QString, QString> &categories);
    void listedFriends(const QMap<QString, QMap<QString, QString> > &friends);
    void listedFriendsOf(const QMap<QString,
                         QMap<QString, QString> > &friendsOf);
    void listedMoods(const QMap<int, QString> &moods);
    void listedPictureKeywords(const QMap<QString, QUrl> &pictureKeywords);
    void fetchedUserInfo();

protected:
    LiveJournal(const QUrl &server, LiveJournalPrivate &dd, QObject *parent = nullptr);

private:
    Q_DECLARE_PRIVATE(LiveJournal)
    Q_PRIVATE_SLOT(d_func(),
                   void slotAddFriend(const QList<QVariant> &, const QVariant &))
    Q_PRIVATE_SLOT(d_func(),
                   void slotAssignFriendToCategory(const QList<QVariant> &, const QVariant &))
    Q_PRIVATE_SLOT(d_func(),
                   void slotCreatePost(const QList<QVariant> &, const QVariant &))
    Q_PRIVATE_SLOT(d_func(),
                   void slotDeleteFriend(const QList<QVariant> &, const QVariant &))
//     Q_PRIVATE_SLOT( d_func(),
//                     void slotExpireCookie( const QList<QVariant> &, const QVariant & ) )
    Q_PRIVATE_SLOT(d_func(),
                   void slotFetchPost(const QList<QVariant> &, const QVariant &))
    Q_PRIVATE_SLOT(d_func(),
                   void slotFetchUserInfo(const QList<QVariant> &, const QVariant &))
//     Q_PRIVATE_SLOT( d_func(),
//                     void slotGenerateCookie( const QList<QVariant> &, const QVariant & ) )
    Q_PRIVATE_SLOT(d_func(),
                   void slotListCategories(const QList<QVariant> &, const QVariant &))
    Q_PRIVATE_SLOT(d_func(),
                   void slotListFriends(const QList<QVariant> &, const QVariant &))
    Q_PRIVATE_SLOT(d_func(),
                   void slotListFriendsOf(const QList<QVariant> &, const QVariant &))
    Q_PRIVATE_SLOT(d_func(),
                   void slotListMoods(const QList<QVariant> &, const QVariant &))
    Q_PRIVATE_SLOT(d_func(),
                   void slotListPictureKeywords(const QList<QVariant> &, const QVariant &))
    Q_PRIVATE_SLOT(d_func(),
                   void slotListRecentPosts(const QList<QVariant> &, const QVariant &))
    Q_PRIVATE_SLOT(d_func(),
                   void slotModifyPost(const QList<QVariant> &, const QVariant &))
    Q_PRIVATE_SLOT(d_func(),
                   void slotRemovePost(const QList<QVariant> &, const QVariant &))
    Q_PRIVATE_SLOT(d_func(),
                   void slotError(int, const QString &, const QVariant &))
};
// Q_DECLARE_OPERATORS_FOR_FLAGS( LiveJournal::GenerateCookieOptions )

} //namespace KBlog
#endif
