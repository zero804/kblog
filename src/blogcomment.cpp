/*
  This file is part of the kblog library.

  SPDX-FileCopyrightText: 2006-2007 Christian Weilbach <christian_weilbach@web.de>
  SPDX-FileCopyrightText: 2007 Mike McQuaid <mike@mikemcquaid.com>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blogcomment.h"
#include "blogcomment_p.h"

namespace KBlog
{

BlogComment::BlogComment(const BlogComment &c)
    : d_ptr(new BlogCommentPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->mTitle = c.title();
    d_ptr->mContent = c.content();
    d_ptr->mEmail = c.email();
    d_ptr->mName = c.name();
    d_ptr->mCommentId = c.commentId();
    d_ptr->mUrl = c.url();
    d_ptr->mError = c.error();
    d_ptr->mStatus = c.status();
    d_ptr->mModificationDateTime = c.modificationDateTime();
    d_ptr->mCreationDateTime = c.creationDateTime();
}

BlogComment::BlogComment(const QString &commentId)
    : d_ptr(new BlogCommentPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->mStatus = New;
    d_ptr->mCommentId = commentId;
}

BlogComment::~BlogComment()
{
    delete d_ptr;
}

QString BlogComment::title() const
{
    return d_ptr->mTitle;
}

void BlogComment::setTitle(const QString &title)
{
    d_ptr->mTitle = title;
}

QString BlogComment::content() const
{
    return d_ptr->mContent;
}

void BlogComment::setContent(const QString &content)
{
    d_ptr->mContent = content;
}

QString BlogComment::commentId() const
{
    return d_ptr->mCommentId;
}

void BlogComment::setCommentId(const QString &commentId)
{
    d_ptr->mCommentId = commentId;
}

QString BlogComment::email() const
{
    return d_ptr->mEmail;
}

void BlogComment::setEmail(const QString &email)
{
    d_ptr->mEmail = email;
}

QString BlogComment::name() const
{
    return d_ptr->mName;
}

void BlogComment::setName(const QString &name)
{
    d_ptr->mName = name;
}

QUrl BlogComment::url() const
{
    return d_ptr->mUrl;
}

void BlogComment::setUrl(const QUrl &url)
{
    d_ptr->mUrl = url;
}

QDateTime BlogComment::modificationDateTime() const
{
    return d_ptr->mModificationDateTime;
}

void BlogComment::setModificationDateTime(const QDateTime &datetime)
{
    d_ptr->mModificationDateTime = datetime;
}

QDateTime BlogComment::creationDateTime() const
{
    return d_ptr->mCreationDateTime;
}

void BlogComment::setCreationDateTime(const QDateTime &datetime)
{
    d_ptr->mCreationDateTime = datetime;
}

BlogComment::Status BlogComment::status() const
{
    return d_ptr->mStatus;
}

void BlogComment::setStatus(BlogComment::Status status)
{
    d_ptr->mStatus = status;
}

QString BlogComment::error() const
{
    return d_ptr->mError;
}

void BlogComment::setError(const QString &error)
{
    d_ptr->mError = error;
}

BlogComment &BlogComment::operator=(const BlogComment &c)
{
    BlogComment copy(c);
    swap(copy);
    return *this;
}

} // namespace KBlog
