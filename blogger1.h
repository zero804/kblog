/*
  This file is part of the kblog library.

  Copyright (c) 2004 Reinhold Kainhofer <reinhold@kainhofer.com>
  Copyright (c) 2006 Christian Weilbach <christian@whiletaker.homeip.net>

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

#ifndef KBLOG_BLOGGER1_H
#define KBLOG_BLOGGER1_H

#include <kblog/blog.h>

class KUrl;
class Blogger1Private;

/**
  @file
  This file is part of the  for accessing Blog Servers
  and defines the Blogger1 class.

  @author Reinhold Kainhofer \<reinhold\@kainhofer.com\>
  @author Christian Weilbach \<christian\@whiletaker.homeip.net\>
*/

namespace KBlog {

/**
   @brief
   A class that can be used for access to Blogger  1.0 blogs.
   Almost every blog server supports Blogger  1.0. Compared to
   MetaWeblog  it is not as functional and is obsolete compared to
   GData which uses Atom instead of Xml-Rpc.

   @code
   Blog* myblog = new Blogger1("http://example.com/xmlrpc/gateway.php");
   KBlog::BlogPosting *post = new BlogPosting();
   post->setUserId( "some_user_id" );
   post->setTitle( "This is the title." );
   post->setContent( "Here is some the content..." );
   myblog->createPosting( posting );
   @endcode

   @author Christian Weilbach \<christian\@whiletaker.homeip.net\>
   @author Reinhold Kainhofer \<reinhold\@kainhofer.com\>
*/
class KBLOG_EXPORT Blogger1 : public Blog
{
  Q_OBJECT
  public:
    /**
      Create an object for Blogger  1.0

      @param server is the url for the xmlrpc gateway.
      @param parent the parent object.
    */
    explicit Blogger1( const KUrl &server, QObject *parent = 0 );

    /**
       Destroy the object.
    */
    ~Blogger1();

    /**
      Returns the  of the inherited object.
    */
    QString interfaceName() const;

    /**
       Set the Url of the server.

       @param server is the server Url.
    */
    void setUrl( const KUrl &server );

    /**
        Get information about the user from the blog. Note: This is not
        supported on the server side.
        @see void fetchedUserInfo( const QString &nickname,
                const QString &userid, const QString &email )
    */
    void fetchUserInfo();

    /**
      List the blogs available for this authentication on the server.
      @see void blogInfoRetrieved( const QString &id, const QString &name )
    */
    void listBlogs();

    /**
      List recent postings on the server.

      @see     void listedPosting( KBlog::BlogPosting &posting )
      @see     void fetchedPosting( KBlog::BlogPosting &posting )
      @see     void listRecentPostingsFinished()
    */
    void listRecentPostings( int number );

    /**
      Fetch the Posting with postingId.

      @param postingId is the id of the posting on the server.

      @see  void fetchedPosting( KBlog::BlogPosting &posting )
    */
    void fetchPosting( KBlog::BlogPosting *posting ); //FIXME docs

    /**
      Modify a posting on server.

      @param posting is used to send the modified posting including
      the correct postingId from it to the server.
    */
    void modifyPosting( KBlog::BlogPosting *posting );

    /**
      Create a new posting on server.

      @param posting is send to the server.
    */
    void createPosting( KBlog::BlogPosting *posting );

    /**
      Remove a posting from the server.

      @param postingId is the id of the posting to remove.

      @see void removePosting( KBlog::BlogPosting *posting )
      @see void modifiedPosting( void modified )
    */
    void removePosting( KBlog::BlogPosting *posting ); //FIXME docs

  protected:
    Blogger1( const KUrl &server, Blogger1Private &dd, QObject *parent = 0 );

  private:
    Q_DECLARE_PRIVATE(Blogger1)
};

} //namespace KBlog
#endif