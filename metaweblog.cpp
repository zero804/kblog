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

#include "metaweblog.h"
#include "metaweblog_p.h"
#include "blogposting.h"
#include "blogmedia.h"

#include <kxmlrpcclient/client.h>
#include <KDebug>
#include <KLocale>
#include <KDateTime>

using namespace KBlog;

MetaWeblog::MetaWeblog( const KUrl &server, QObject *parent )
  : Blogger1( server, *new MetaWeblogPrivate, parent )
{
  setUrl( server );
}

MetaWeblog::MetaWeblog( const KUrl &server, MetaWeblogPrivate &dd,
                        QObject *parent )
  : Blogger1( server, dd, parent )
{
  setUrl( server );
}

MetaWeblog::~MetaWeblog()
{
  kDebug(5323) << "~MetaWeblog()";
}

QString MetaWeblog::interfaceName() const
{
  return QLatin1String( "MetaWeblog" );
}

void MetaWeblog::listRecentPostings( int number )
{
    Q_D(MetaWeblog);
    kDebug(5323) << "Fetching List of Posts...";
    QList<QVariant> args( d->defaultArgs( blogId() ) );
    args << QVariant( number );
    d->mXmlRpcClient->call(
      "metaWeblog.getRecentPosts", args,
      this, SLOT( slotListRecentPostings( const QList<QVariant>&, const QVariant& ) ),
      this, SLOT( slotError( int, const QString&, const QVariant& ) ), QVariant( number ) );
}

void MetaWeblog::listCategories()
{
    Q_D(MetaWeblog);
    kDebug(5323) << "Fetching List of Categories...";
    QList<QVariant> args( d->defaultArgs( blogId() ) );
    d->mXmlRpcClient->call(
      "metaWeblog.getCategories", args,
      this, SLOT( slotListCategories( const QList<QVariant>&, const QVariant& ) ),
      this, SLOT ( slotError( int, const QString&, const QVariant& ) ) );
}

void MetaWeblog::fetchPosting( KBlog::BlogPosting *posting )
{
  Q_D(MetaWeblog);
  if ( !posting ) {
    kError(5323) << "MetaWeblog::fetchPosting: posting is a null pointer";
    emit error ( Other, i18n( "Posting is a null pointer." ) );
    return;
  }
  unsigned int i = d->mCallCounter++;
  d->mCallMap[ i ] = posting;
  kError(5323) << "Fetching Posting with url" << posting->postingId();
  QList<QVariant> args( d->defaultArgs( posting->postingId() ) );
  d->mXmlRpcClient->call(
    "metaWeblog.getPost", args,
    this, SLOT( slotFetchPosting( const QList<QVariant>&, const QVariant& ) ),
    this, SLOT( slotError( int, const QString&, const QVariant& ) ), QVariant( i ) );
}

void MetaWeblog::modifyPosting( KBlog::BlogPosting *posting )
{
  Q_D(MetaWeblog);
  if ( !posting ) {
    kError(5323) << "MetaWeblog::modifyPosting: posting is a null pointer";
    emit error ( Other, i18n( "Posting is a null pointer." ) );
    return;
  }
  unsigned int i = d->mCallCounter++;
  d->mCallMap[ i ] = posting;
  kDebug(5323) << "Uploading Posting with postId" << posting->postingId();

  QList<QVariant> args( d->defaultArgs( posting->postingId() ) );
  QMap<QString, QVariant> map;
  map["categories"] = posting->categories();
  map["description"] = posting->content();
  map["title"] = posting->title();
  map["lastModified"] = posting->modificationDateTime().toUtc().dateTime();
  args << map;
  args << QVariant( !posting->isPrivate() );
  d->mXmlRpcClient->call(
    "metaWeblog.editPost", args,
     this, SLOT( slotModifyPosting( const QList<QVariant>&, const QVariant& ) ),
     this, SLOT ( slotError( int, const QString&, const QVariant& ) ), QVariant( i ) );
}

void MetaWeblog::createPosting( KBlog::BlogPosting *posting )
{
  Q_D(MetaWeblog);
  if ( !posting ) {
    kError(5323) << "MetaWeblog::createPosting: posting is a null pointer";
    emit error ( Other, i18n( "Posting is a null pointer." ) );
    return;
  }
  unsigned int i = d->mCallCounter++;
  d->mCallMap[ i ] = posting;
  kDebug(5323) << "Creating new Posting with blogId" << blogId();
  QList<QVariant> args( d->defaultArgs( blogId() ) );
  QMap<QString, QVariant> map;
  map["categories"] = posting->categories();
  map["description"] = posting->content();
  map["title"] = posting->title();
  map["dateCreated"] = posting->creationDateTime().toUtc().dateTime();
  args << map;
  args << QVariant( !posting->isPrivate() );
  d->mXmlRpcClient->call (
    "metaWeblog.newPost", args,
    this, SLOT( slotCreatePosting( const QList<QVariant>&, const QVariant& ) ),
    this, SLOT ( slotError( int, const QString&, const QVariant& ) ), QVariant( i ) );
}

void MetaWeblog::createMedia( KBlog::BlogMedia *media )
{
  Q_D(MetaWeblog);
  if ( !media ) {
    kError(5323) << "MetaWeblog::createMedia: media is a null pointer";
    emit error ( Other, i18n( "Media is a null pointer." ) );
    return;
  }
  unsigned int i = d->mCallMediaCounter++;
  d->mCallMediaMap[ i ] = media;
  kDebug(5323) << "MetaWeblog::createMedia: name="<< media->name();
  QList<QVariant> args( d->defaultArgs( blogId() ) );
  QMap<QString, QVariant> map;
  QList<QVariant> list;
  map["name"] = media->name();
  map["type"] = media->mimetype();
  map["bits"] = media->data();
  args << map;
  d->mXmlRpcClient->call(
    "metaWeblog.newMediaObject", args,
    this, SLOT( slotCreateMedia( const QList<QVariant>&, const QVariant& ) ),
    this, SLOT( slotError( int, const QString&, const QVariant& ) ), QVariant( i ) );

}

MetaWeblogPrivate::MetaWeblogPrivate()
{
  mCallMediaCounter=1;
}

MetaWeblogPrivate::~MetaWeblogPrivate()
{
  kDebug(5323) << "~MetaWeblogPrivate()";
}

QList<QVariant> MetaWeblogPrivate::defaultArgs( const QString &id )
{
  Q_Q(MetaWeblog);
  QList<QVariant> args;
  if( !id.isEmpty() )
    args << QVariant( id );
  args << QVariant( q->username() )
          << QVariant( q->password() );
  return args;
}

void MetaWeblogPrivate::slotListCategories( const QList<QVariant> &result,
                                                              const QVariant &id )
{
  Q_Q(MetaWeblog);
  Q_UNUSED( id );

  QList<QMap<QString,QString> > categoriesList;

  kDebug(5323) << "MetaWeblogPrivate::slotListCategories";
  kDebug(5323) << "TOP:" << result[0].typeName();
  if ( result[0].type() != QVariant::Map &&
       result[0].type() != QVariant::List ) {
    // include fix for not metaweblog standard compatible apis with
    // array of structs instead of struct of structs, e.g. wordpress
    kError(5323) << "Could not list categories out of the result from the server.";
    emit q->error( MetaWeblog::ParsingError,
                        i18n( "Could not list categories out of the result "
                              "from the server." ) );
  } else {
    if ( result[0].type() == QVariant::Map ) {
      const QMap<QString, QVariant> serverMap = result[0].toMap();
      const QList<QString> serverKeys = serverMap.keys();

      QList<QString>::ConstIterator it = serverKeys.begin();
      QList<QString>::ConstIterator end = serverKeys.end();
      for ( ; it != end; ++it ) {
        kDebug(5323) << "MIDDLE:" << ( *it );
        QMap<QString,QString> category;
        const QMap<QString, QVariant> serverCategory = serverMap[*it].toMap();
        category["name"]= ( *it );
        category["description"] = serverCategory[ "description" ].toString();
        category["htmlUrl"] = serverCategory[ "htmlUrl" ].toString();
        category["rssUrl"] = serverCategory[ "rssUrl" ].toString();
        categoriesList.append( category );
        }
        emit q->listedCategories( categoriesList );
        kDebug(5323) << "Emitting listedCategories";
      }
    }
    if ( result[0].type() == QVariant::List ) {
      // include fix for not metaweblog standard compatible apis with
      // array of structs instead of struct of structs, e.g. wordpress
      const QList<QVariant> serverList = result[0].toList();
      QList<QVariant>::ConstIterator it = serverList.begin();
      QList<QVariant>::ConstIterator end = serverList.end();
      for ( ; it != end; ++it ) {
        kDebug(5323) << "MIDDLE:" << ( *it ).typeName();
        QMap<QString,QString> category;
        const QMap<QString, QVariant> serverCategory = ( *it ).toMap();
        category[ "name" ] = serverCategory["categoryName"].toString();
        category["description"] = serverCategory[ "description" ].toString();
        category["htmlUrl"] = serverCategory[ "htmlUrl" ].toString();
        category["rssUrl"] = serverCategory[ "rssUrl" ].toString();
        categoriesList.append( category );
      }
      kDebug(5323) << "Emitting listedCategories()";
      emit q->listedCategories( categoriesList );
    }
  }

void MetaWeblogPrivate::slotListRecentPostings( const QList<QVariant> &result,
                                                            const QVariant &id )
{
  Q_Q(MetaWeblog);

  int count = id.toInt();

  QList <BlogPosting> fetchedPostingList;

  kDebug(5323) << "MetaWeblog::slotListRecentPostings";
  kDebug(5323) << "TOP:" << result[0].typeName();
  if ( result[0].type() != QVariant::List ) {
    kError(5323) << "Could not fetch list of postings out of the"
                 << "result from the server.";
    emit q->error( MetaWeblog::ParsingError,
                        i18n( "Could not fetch list of postings out of the "
                              "result from the server." ) );
  } else {
    const QList<QVariant> postReceived = result[0].toList();
    QList<QVariant>::ConstIterator it = postReceived.begin();
    QList<QVariant>::ConstIterator end = postReceived.end();
    for ( ; it != end; ++it ) {
      BlogPosting posting;
      kDebug(5323) << "MIDDLE:" << ( *it ).typeName();
      const QMap<QString, QVariant> postInfo = ( *it ).toMap();
      if ( readPostingFromMap( &posting, postInfo ) ) {
        kDebug(5323) << "Emitting listedPosting( posting.postingId()="
                     << posting.postingId() << ");";
        fetchedPostingList << posting;
      } else {
        kError(5323) << "readPostingFromMap failed!";
        emit q->error( MetaWeblog::ParsingError, i18n( "Could not read posting." ) );
      }
      if( --count == 0 ) break;
    }
  } //FIXME should we emit here? (see below, too)
  kDebug(5323) << "Emitting listedRecentPostings()";
  emit q->listedRecentPostings( fetchedPostingList );
}

void MetaWeblogPrivate::slotFetchPosting( const QList<QVariant> &result,
                                                            const QVariant &id )
{
  Q_Q(MetaWeblog);

  KBlog::BlogPosting* posting = mCallMap[ id.toInt() ];
  mCallMap.remove( id.toInt() );

  kDebug(5323) << "MetaWeblog::slotFetchPosting";
  //array of structs containing ISO.8601
  // dateCreated, String userid, String postid, String content;
  // TODO: Time zone for the dateCreated!
  kDebug(5323) << "TOP:" << result[0].typeName();
  if ( result[0].type() != QVariant::Map ) {
    kError(5323) << "Could not fetch posting out of the result from the server.";
    emit q->error( MetaWeblog::ParsingError,
                       i18n( "Could not fetch posting out of the "
                             "result from the server." ), posting );
  } else {
    const QMap<QString, QVariant> postInfo = result[0].toMap();
    if ( readPostingFromMap( posting, postInfo ) ) {
      kDebug(5323) << "Emitting fetchedPosting( posting.postingId()="
                   << posting->postingId() << ");";
      posting->setStatus( BlogPosting::Fetched );
      emit q->fetchedPosting( posting );
    } else {
      kError(5323) << "readPostingFromMap failed!";
      emit q->error( MetaWeblog::ParsingError,
                         i18n( "Could not read posting." ), posting );
    }
  }
}

void MetaWeblogPrivate::slotCreatePosting( const QList<QVariant> &result,
                                                             const QVariant &id )
{
  Q_Q(MetaWeblog);

  KBlog::BlogPosting* posting = mCallMap[ id.toInt() ];
  mCallMap.remove( id.toInt() );

  kDebug(5323) << "MetaWeblog::slotCreatePosting";
  //array of structs containing ISO.8601
  // dateCreated, String userid, String postid, String content;
  // TODO: Time zone for the dateCreated!
  kDebug(5323) << "TOP:" << result[0].typeName();
  if ( result[0].type() != QVariant::String ) {
    kError(5323) << "Could not read the postingId, not a string.";
    emit q->error( MetaWeblog::ParsingError,
                       i18n( "Could not read the postingId, not a string." ), posting );
  } else {
     posting->setPostingId( result[0].toString() );
     posting->setStatus( BlogPosting::Created );
     emit q->createdPosting( posting );
    kDebug(5323) << "emitting createdPosting(" << result[0].toString() << ")";
  }
}

void MetaWeblogPrivate::slotModifyPosting( const QList<QVariant> &result,
                                                             const QVariant &id )
{
  Q_Q(MetaWeblog);

  KBlog::BlogPosting* posting = mCallMap[ id.toInt() ];
  mCallMap.remove( id.toInt() );

  kDebug(5323) << "MetaWeblogPrivate::slotModifyPosting";
  //array of structs containing ISO.8601
  // dateCreated, String userid, String postid, String content;
  // TODO: Time zone for the dateCreated!
  kDebug(5323) << "TOP:" << result[0].typeName();
  if ( result[0].type() != QVariant::Bool ) {
    kError(5323) << "Could not read the result, not a boolean.";
    emit q->error( MetaWeblog::ParsingError,
                       i18n( "Could not read the result, not a boolean." ), posting );
  } else {
    posting->setStatus( BlogPosting::Modified );
    emit q->modifiedPosting( posting );
    kDebug(5323) << "emitting modifiedPosting()";
  }
}

void MetaWeblogPrivate::slotCreateMedia( const QList<QVariant> &result,
                                                           const QVariant &id )
{
  Q_Q(MetaWeblog);

  KBlog::BlogMedia* media = mCallMediaMap[ id.toInt() ];
  mCallMediaMap.remove( id.toInt() );

  kDebug(5323) << "MetaWeblogPrivate::slotCreateMedia, no error!";
  kDebug(5323) << "TOP:" << result[0].typeName();
  if ( result[0].type() != 8 ) {
    kError(5323) << "Could not read the result, not a map.";
    emit q->error( MetaWeblog::ParsingError,
                       i18n( "Could not read the result, not a map." ) );
  } else {
    const QMap<QString, QVariant> resultStruct = result[0].toMap();
    const QString url = resultStruct["url"].toString();
    kDebug(5323) << "MetaWeblog::slotCreateMedia url=" << url;

    if ( !url.isEmpty() ) {
      media->setUrl( KUrl( url ) );
      media->setStatus( BlogMedia::Created );
      emit q->createdMedia( media );
      kDebug(5323) << "Emitting createdMedia( url=" << url  << ");";
    }
  }
}

void MetaWeblogPrivate::slotError( int number,
                                                     const QString &errorString,
                                                     const QVariant &id )
{
  Q_Q(MetaWeblog);
  Q_UNUSED( number );
  BlogPosting *posting = mCallMap[ id.toInt() ];

  emit q->error( MetaWeblog::XmlRpc, errorString, posting );
}

bool MetaWeblogPrivate::readPostingFromMap( BlogPosting *post,
                                                        const QMap<QString, QVariant> &postInfo )
{
  // FIXME: integrate error handling
  if ( !post ) {
    return false;
  }
  QStringList mapkeys = postInfo.keys();
  kDebug(5323) << endl << "Keys:" << mapkeys.join( ", " );
  kDebug(5323) << endl;

  KDateTime dt =
    KDateTime( postInfo["dateCreated"].toDateTime(), KDateTime::UTC );
  if ( dt.isValid() && !dt.isNull() ) {
    post->setCreationDateTime( dt );
  }

  dt =
    KDateTime( postInfo["lastModified"].toDateTime(), KDateTime::UTC );
  if ( dt.isValid() && !dt.isNull() ) {
    post->setModificationDateTime( dt );
  }

  post->setPostingId( postInfo["postid"].toString() );

  QString title( postInfo["title"].toString() );
  QString description( postInfo["description"].toString() );
  QStringList categories( postInfo["categories"].toStringList() );

  post->setTitle( title );
  post->setContent( description );
  if ( !categories.isEmpty() ){
    kDebug(5323) << "Categories:" << categories;
    post->setCategories( categories );
  }
  return true;
}

#include "metaweblog.moc"
