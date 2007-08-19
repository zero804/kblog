/*
  This file is part of the kblog library.

  Copyright (c) 2004 Reinhold Kainhofer <reinhold@kainhofer.com>
  Copyright (c) 2006-2007 Christian Weilbach <christian_weilbach@web.de>
  Copyright (c) 2007 Mike Arthur <mike@mikearthur.co.uk>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#ifndef KBLOG_BLOG_H
#define KBLOG_BLOG_H

#include <kblog/kblog_export.h>

#include <QtCore/QObject>

template <class T,class S> class QMap;

class KTimeZone;
class KUrl;

/**
  This is the main interface for blog backends
  @author Ian Reinhart Geiser, Reinhold Kainhofer, Christian Weilbach
*/

/**
  @file
  This file is part of the  for accessing Blog Servers
  and defines the BlogPosting, BlogMedia, and Blog class.

  @author Reinhold Kainhofer \<reinhold\@kainhofer.com\>
  @author Christian Weilbach \<christian_weilbach\@web.de\>
*/

/** Namespace for blog related classes. */
namespace KBlog {

class BlogPosting;
class BlogPostingComment;
class BlogMedia;
class BlogPrivate;

/**
  @brief
  A virtual basis class that represents a connection to a blog server.
  This is the main interface to the blog client library.

  @author Christian Weilbach \<christian_weilbach\@web.de\>
  @author Reinhold Kainhofer \<reinhold\@kainhofer.com\>
*/

class KBLOG_EXPORT Blog : public QObject
{
  Q_OBJECT
  public:
    /**
    Construtor used by the  implementations.

    @param server the gateway url of the server.
    @param parent the parent of this object, defaults to NULL.
    @param applicationName the client application's name to use in the
    user agent, defaults to empty string.
    @param applicationVersion the client application's version to use in the
    user agent, defaults to empty string.
     */
    explicit Blog( const KUrl &server, QObject *parent = 0,
                   const QString &applicationName = QString(),
                   const QString &applicationVersion = QString() );

    /**
      Destroys the Blog object.
    */
    virtual ~Blog();

    /**
      Enumeration for possible errors.
    */
    enum ErrorType {
      /** This type specifies an error in the KXmlRpcClient. */
      XmlRpc,
      /** This type specifies an error in Syndication. */
      Atom,
      /** This type specifies a parsing error. */
      ParsingError,
      /** This type specifies an error on authentication. */
      AuthenticationError,
      /** This type specifies an error when the method is not
      supported by the API you chose. */
      NotSupported,
      /** This type specifies a unique error type not yet specified. */
      Other
    };

    /**
      Returns user agent used in requests.
    */
    QString userAgent() const;

    /**
      Returns the  of the inherited object.
    */
    virtual QString interfaceName() const = 0;

    /**
      Sets the blog id of the Server.

      @param blogId
    */
    virtual void setBlogId( const QString &blogId );

    /**
      Returns the blog id.

      @return blogId
    */
    QString blogId() const;

    /**
      Sets the password for the blog.
      @param pass is a QString containing the blog password.

      @see password();
    */
    virtual void setPassword( const QString &pass );

    /**
      Returns the password of the blog.
      @see setPassword( const QString & );
    */
    QString password() const;

    /**
      Sets the user's authentication name for the blog.
      @param userName is a QString containing the blog username.

      @see username()
    */
    virtual void setUsername( const QString &userName );

    /**
       Returns the user's name of the blog.
       @see setUsername(const QString &)
    */
    QString username() const;

    /**
      Sets the URL for the blog.
      @param url is the blog URL.

      @see url()
    */
    virtual void setUrl( const KUrl &url );

    /**
      Get the URL for the blog.
      @see setUrl(const KUrl &)
    */
    KUrl url() const;

    /**
      Sets the time zone of the blog server.
      @param tz time zone of the server
      @see timeZone()
    */
    virtual void setTimeZone( const KTimeZone &tz );

    /**
      Get the time zone of the blog server.
      @see void setTimeZone()
    */
    KTimeZone timeZone();

    /**
      List recent postings on the server.

      @param number The number of postings to fetch. Latest first.

      @see listedPosting()
      @see fetchedPosting()
      @see listRecentPostingsFinished()
    */
    virtual void listRecentPostings( int number ) = 0;

    /**
      Fetch the Posting with postingId.
      @param posting is the id of the posting on the server.

      @see fetchedPosting()
    */
    virtual void fetchPosting( KBlog::BlogPosting *posting ) = 0;

    /**
      Modify a posting on server.

      @param posting is used to send the modified posting including the
      correct postingId from it to the server.
    */
    virtual void modifyPosting( KBlog::BlogPosting *posting ) = 0;

    /**
      Create a new posting on server.

      @param posting is send to the server.
    */
    virtual void createPosting( KBlog::BlogPosting *posting ) = 0;

    /**
      Remove a posting from the server.

      @param posting* is the BlogPosting to remove.
    */
    virtual void removePosting( KBlog::BlogPosting *posting ) = 0;

  Q_SIGNALS:

    /**
      This signal is emitted when a listRecentPostings() job fetches a posting
      from the blogging server.

      @param postings is the list of postings.

      @see listRecentPostings()
    */
    void listedRecentPostings(
        const QList<KBlog::BlogPosting>& postings );

    /**
      This signal is emitted when a createPosting() job creates a posting
      on the blogging server.

      @param posting is the created posting.

      @see createPosting()
    */
    void createdPosting( KBlog::BlogPosting *posting );

    /**
      This signal is emitted when a fetchPosting() job fetches a posting
      from the blogging server.

      @param posting is the fetched posting.

      @see fetchPosting()
    */
    void fetchedPosting( KBlog::BlogPosting *posting );

    /**
      This signal is emitted when a modifyPosting() job modifies a posting
      on the blogging server.

      @param posting is the modified posting.

      @see modifyPosting()
    */
    void modifiedPosting( KBlog::BlogPosting *posting );

    /**
      This signal is emitted when a removePosting() job removes a posting
      from the blogging server.

      @param posting is the removed posting.

      @see removePosting()
    */
    void removedPosting( KBlog::BlogPosting *posting );

    /**
      All xml parsing and all structural problems will emit an error.

     @param type The type of the error.
     @param errorMessage The string containing the error message.
     @param posting The posting to which the error belongs.

      @see ErrorType
    */
    void error( KBlog::Blog::ErrorType type,
                        const QString &errorMessage, KBlog::BlogPosting* posting = 0 );

    /**
      All xml parsing and all structural problems will emit an error.

     @param type The type of the error.
     @param errorMessage The string containing the error message.
     @param media The media to which the error belongs.

      @see ErrorType
    */
    void error( KBlog::Blog::ErrorType type,
                        const QString &errorMessage, KBlog::BlogMedia* media );

    /**
      All xml parsing and all structural problems will emit an error.

     @param type The type of the error.
     @param errorMessage The string containing the error message.
     @param posting The posting the comment belongs to.
     @param comment The comment the error belongs to.

      @see ErrorType
    */
    void error( KBlog::Blog::ErrorType type,
                        const QString &errorMessage, KBlog::BlogPosting* posting, 
                        KBlog::BlogPostingComment* comment );

  protected:
    BlogPrivate * const d_ptr;
    /**
      Constructor needed for private inheritance.
    */
    Blog( const KUrl &server, BlogPrivate &dd, QObject *parent = 0,
          const QString &applicationName = QString(),
          const QString &applicationVersion = QString() );

  private:
    void setUserAgent( const QString &applicationName,
                       const QString &applicationVersion );
    Q_DECLARE_PRIVATE(Blog)
};

} //namespace KBlog
#endif
