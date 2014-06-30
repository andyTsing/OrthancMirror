/**
 * Orthanc - A Lightweight, RESTful DICOM Store
 * Copyright (C) 2012-2014 Medical Physics Department, CHU of Liege,
 * Belgium
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * In addition, as a special exception, the copyright holders of this
 * program give permission to link the code of its release with the
 * OpenSSL project's "OpenSSL" library (or with modified versions of it
 * that use the same license as the "OpenSSL" library), and distribute
 * the linked executables. You must obey the GNU General Public License
 * in all respects for all of the code used other than "OpenSSL". If you
 * modify file(s) with this exception, you may extend this exception to
 * your version of the file(s), but you are not obligated to do so. If
 * you do not wish to do so, delete this exception statement from your
 * version. If you delete this exception statement from all source files
 * in the program, then also delete it here.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/


#pragma once

#include "RestApi.h"

#include "../OrthancException.h"

#include <list>
#include <set>

namespace Orthanc
{
  class RestApiHierarchy
  {
  private:
    class Handlers
    {
    private:
      RestApiGetCall::Handler  getHandler_;
      RestApiPostCall::Handler  postHandler_;
      RestApiPutCall::Handler  putHandler_;
      RestApiDeleteCall::Handler  deleteHandler_;

    public:
      Handlers();

      bool HasHandler(HttpMethod method) const;

      RestApiGetCall::Handler GetGetHandler() const;

      RestApiPutCall::Handler GetPutHandler() const;

      RestApiPostCall::Handler GetPostHandler() const;

      RestApiDeleteCall::Handler GetDeleteHandler() const;

      void Register(RestApiGetCall::Handler handler)
      {
        getHandler_ = handler;
      }

      void Register(RestApiPutCall::Handler handler)
      {
        putHandler_ = handler;
      }

      void Register(RestApiPostCall::Handler handler)
      {
        postHandler_ = handler;
      }

      void Register(RestApiDeleteCall::Handler handler)
      {
        deleteHandler_ = handler;
      }

      bool IsEmpty() const;
    };


    typedef std::map<std::string, RestApiHierarchy*>  Children;
    typedef bool (*ResourceCallback) (Handlers& handlers,
                                      const UriComponents& uri,
                                      const RestApiPath::Components& components,
                                      const UriComponents& trailing,
                                      void* call);

    Handlers  handlers_;
    Children  children_;
    Children  wildcardChildren_;
    Handlers  universalHandlers_;


    static RestApiHierarchy& AddChild(Children& children,
                                      const std::string& name);

    static void DeleteChildren(Children& children);

    template <typename Handler>
    void RegisterInternal(const RestApiPath& path,
                          Handler handler,
                          size_t level);

    bool LookupHandler(RestApiPath::Components& components,
                       const UriComponents& uri,
                       ResourceCallback callback,
                       size_t level,
                       void* call);

    bool GetDirectory(Json::Value& result,
                      const UriComponents& uri,
                      size_t level);
                     
    static bool GetCallback(Handlers& handlers,
                            const UriComponents& uri,
                            const RestApiPath::Components& components,
                            const UriComponents& trailing,
                            void* call);

    static bool PostCallback(Handlers& handlers,
                             const UriComponents& uri,
                             const RestApiPath::Components& components,
                             const UriComponents& trailing,
                             void* call);

    static bool PutCallback(Handlers& handlers,
                            const UriComponents& uri,
                            const RestApiPath::Components& components,
                            const UriComponents& trailing,
                            void* call);

    static bool DeleteCallback(Handlers& handlers,
                               const UriComponents& uri,
                               const RestApiPath::Components& components,
                               const UriComponents& trailing,
                               void* call);
                       

  public:
    ~RestApiHierarchy();

    void Register(const std::string& uri,
                  RestApiGetCall::Handler handler);

    void Register(const std::string& uri,
                  RestApiPutCall::Handler handler);

    void Register(const std::string& uri,
                  RestApiPostCall::Handler handler);

    void Register(const std::string& uri,
                  RestApiDeleteCall::Handler handler);

    void CreateSiteMap(Json::Value& target) const;

    bool GetDirectory(Json::Value& result,
                      const UriComponents& uri)
    {
      return GetDirectory(result, uri, 0);
    }

    bool Handle(RestApiGetCall& call,
                const UriComponents& uri);

    bool Handle(RestApiPutCall& call,
                const UriComponents& uri);

    bool Handle(RestApiPostCall& call,
                const UriComponents& uri);

    bool Handle(RestApiDeleteCall& call,
                const UriComponents& uri);
  };
}