/*
  This file is part of the kblog library.

  SPDX-FileCopyrightText: 2006-2007 Christian Weilbach <christian_weilbach@web.de>
  SPDX-FileCopyrightText: 2007 Mike McQuaid <mike@mikemcquaid.com>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blogpost.h"
#include "blogpost_p.h"

#include "blog.h"

#include <QStringList>

namespace KBlog
{

BlogPost::BlogPost(const KBlog::BlogPost &post)
    : d_ptr(new BlogPostPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->mPrivate = post.isPrivate();
    d_ptr->mPostId = post.postId();
    d_ptr->mTitle = post.title();
    d_ptr->mContent = post.content();
    d_ptr->mAdditionalContent = post.additionalContent();
    d_ptr->mWpSlug = post.slug();
    d_ptr->mCategories = post.categories();
    d_ptr->mTags = post.tags();
    d_ptr->mMood = post.mood();
    d_ptr->mPermaLink = post.permaLink();
    d_ptr->mSummary = post.summary();
    d_ptr->mLink = post.link();
    d_ptr->mMusic = post.music();
    d_ptr->mTrackBackAllowed = post.isTrackBackAllowed();
    d_ptr->mCommentAllowed = post.isCommentAllowed();
    d_ptr->mError = post.error();
    d_ptr->mJournalId = post.journalId();
    d_ptr->mStatus = post.status();
    d_ptr->mCreationDateTime = post.creationDateTime();
    d_ptr->mModificationDateTime = post.modificationDateTime();
}

BlogPost::BlogPost(const QString &postId)
    : d_ptr(new BlogPostPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->mPrivate = false;
    d_ptr->mPostId = postId;
    d_ptr->mStatus = New;
}

BlogPost::BlogPost(const KCalendarCore::Journal::Ptr &journal)
    : d_ptr(new BlogPostPrivate)
{
    Q_ASSERT(journal);
    d_ptr->q_ptr = this;
    d_ptr->mPrivate = false;
    d_ptr->mPostId = journal->customProperty("KBLOG", "ID");
    d_ptr->mJournalId = journal->uid();
    d_ptr->mStatus = New;
    d_ptr->mTitle = journal->summary();
    if (journal->descriptionIsRich()) {
        d_ptr->mContent = d_ptr->cleanRichText(journal->description());
    } else {
        d_ptr->mContent = journal->description();
    }
    d_ptr->mCategories = journal->categories();
    d_ptr->mCreationDateTime = journal->dtStart();
}

// BlogPost::BlogPost( const KCal::Journal &journal, BlogPostPrivate &dd )
//   : d_ptr( &dd )
// {
//   d_ptr->q_ptr = this;
//   d_ptr->mPrivate = false;
//   d_ptr->mPostId = journal.customProperty( "KBLOG", "ID" );
//   d_ptr->mJournalId = journal.uid();
//   d_ptr->mStatus = New;
//   d_ptr->mTitle = journal.summary();
//   d_ptr->mContent = journal.description();
//   d_ptr->mCategories = journal.categories();
//   d_ptr->mCreationDateTime = journal.dtStart();
// }

BlogPost::~BlogPost()
{
    delete d_ptr;
}

KCalendarCore::Journal::Ptr BlogPost::journal(const Blog &blog) const
{
    QString url = blog.url().url();
    QString username = blog.username();
    QString blogId = blog.blogId();
    // Generate unique ID. Should be unique enough...
    QString id = QStringLiteral("kblog-") + url + QLatin1Char('-') + blogId  + QLatin1Char('-') + username +
                 QLatin1Char('-') + d_ptr->mPostId;
    KCalendarCore::Journal::Ptr journal(new KCalendarCore::Journal());
    journal->setUid(id);
    journal->setSummary(d_ptr->mTitle);
    journal->setCategories(d_ptr->mCategories);
    journal->setDescription(d_ptr->mContent, true);
    journal->setDtStart(d_ptr->mCreationDateTime);
    journal->setCustomProperty("KBLOG", "URL", url);
    journal->setCustomProperty("KBLOG", "USER", blog.username());
    journal->setCustomProperty("KBLOG", "BLOG", blogId);
    journal->setCustomProperty("KBLOG", "ID", d_ptr->mPostId);
    return journal;
}

QString BlogPost::journalId() const
{
    return d_ptr->mJournalId;
}

bool BlogPost::isPrivate() const
{
    return d_ptr->mPrivate;
}

void BlogPost::setPrivate(bool privatePost)
{
    d_ptr->mPrivate = privatePost;
}

QString BlogPost::postId() const
{
    return d_ptr->mPostId;
}

void BlogPost::setPostId(const QString &postId)
{
    d_ptr->mPostId = postId;
}

QString BlogPost::title() const
{
    return d_ptr->mTitle;
}

void BlogPost::setTitle(const QString &title)
{
    d_ptr->mTitle = title;
}

QString BlogPost::content() const
{
    return d_ptr->mContent;
}

void BlogPost::setContent(const QString &content)
{
    d_ptr->mContent = content;
}

// QString BlogPost::abbreviatedContent() const
// {
//   //TODO
//   return 0;
// }
//
// void BlogPost::setAbbreviatedContent( const QString &abbreviatedContent )
// {
//   Q_UNUSED( abbreviatedContent );
//   //TODO
// }

QString BlogPost::additionalContent() const
{
    return d_ptr->mAdditionalContent;
}

void BlogPost::setAdditionalContent(const QString &additionalContent)
{
    d_ptr->mAdditionalContent = additionalContent;
}

QString BlogPost::slug() const
{
    return d_ptr->mWpSlug;
}

void BlogPost::setSlug(const QString &slug)
{
    d_ptr->mWpSlug = slug;
}

QUrl BlogPost::link() const
{
    return d_ptr->mLink;
}

void BlogPost::setLink(const QUrl &link) const
{
    d_ptr->mLink = link;
}

QUrl BlogPost::permaLink() const
{
    return d_ptr->mPermaLink;
}

void BlogPost::setPermaLink(const QUrl &permalink) const
{
    d_ptr->mPermaLink = permalink;
}

bool BlogPost::isCommentAllowed() const
{
    return d_ptr->mCommentAllowed;
}

void BlogPost::setCommentAllowed(bool commentAllowed)
{
    d_ptr->mCommentAllowed = commentAllowed;
}

bool BlogPost::isTrackBackAllowed() const
{
    return d_ptr->mCommentAllowed;
}

void BlogPost::setTrackBackAllowed(bool allowTrackBacks)
{
    d_ptr->mTrackBackAllowed = allowTrackBacks;
}

QString BlogPost::summary() const
{
    return d_ptr->mSummary;
}

void BlogPost::setSummary(const QString &summary)
{
    d_ptr->mSummary = summary;
}

QStringList BlogPost::tags() const
{
    return d_ptr->mTags;
}

void BlogPost::setTags(const QStringList &tags)
{
    d_ptr->mTags = tags;
}

// QList<QUrl> BlogPost::trackBackUrls() const
// {
//   //TODO
//   return QList<QUrl>();
// }
//
// void BlogPost::setTrackBackUrls( const QList<QUrl> &trackBackUrls )
// {
//   Q_UNUSED( trackBackUrls );
//   //TODO
// }

QString BlogPost::mood() const
{
    return d_ptr->mMood;
}

void BlogPost::setMood(const QString &mood)
{
    d_ptr->mMood = mood;
}

QString BlogPost::music() const
{
    return d_ptr->mMusic;
}

void BlogPost::setMusic(const QString &music)
{
    d_ptr->mMusic = music;
}

QStringList BlogPost::categories() const
{
    return d_ptr->mCategories;
}

void BlogPost::setCategories(const QStringList &categories)
{
    d_ptr->mCategories = categories;
}

QDateTime BlogPost::creationDateTime() const
{
    return d_ptr->mCreationDateTime;
}

void BlogPost::setCreationDateTime(const QDateTime &datetime)
{
    d_ptr->mCreationDateTime = datetime;
}

QDateTime BlogPost::modificationDateTime() const
{
    return d_ptr->mModificationDateTime;
}

void BlogPost::setModificationDateTime(const QDateTime &datetime)
{
    d_ptr->mModificationDateTime = datetime;
}

BlogPost::Status BlogPost::status() const
{
    return d_ptr->mStatus;
}

void BlogPost::setStatus(BlogPost::Status status)
{
    d_ptr->mStatus = status;
}

QString BlogPost::error() const
{
    return d_ptr->mError;
}

void BlogPost::setError(const QString &error)
{
    d_ptr->mError = error;
}

BlogPost &BlogPost::operator=(const BlogPost &other)
{
    BlogPost copy(other);
    swap(copy);
    return *this;
}

QString BlogPostPrivate::cleanRichText(QString richText) const
{
    QRegExp getBodyContents(QStringLiteral("<body[^>]*>(.*)</body>"));
    if (getBodyContents.indexIn(richText)) {
        // Get anything inside but excluding the body tags
        richText = getBodyContents.cap(1);
        // Get rid of any whitespace
        richText.remove(QRegExp(QStringLiteral("^\\s+")));
    }
    // Get rid of styled paragraphs
    richText.replace(QRegExp(QStringLiteral("<p style=\"[^\"]*\">")), QStringLiteral("<p>"));

    // If we're left with empty content then return a clean empty string
    if (richText == QLatin1String("<p></p>")) {
        richText.clear();
    }

    return richText;
}

} // namespace KBlog
