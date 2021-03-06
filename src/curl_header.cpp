/* 
 * File:   curl_header.cpp
 * Author: Giuseppe
 * 
 * Created on March 25, 2014, 10:45 PM
 */

#include "curl_header.h"
#include "curl_error.h"
#include <algorithm>

using std::for_each;
using curl::curl_header;
using curl::curl_error;

// Implementation of constructor.
curl_header::curl_header() :  size(0), headers(nullptr) {
    // ... nothing to do here ...
}

// Implementation of initializer list constructor.
curl_header::curl_header(initializer_list<string> headers) {
    for_each(headers.begin(),headers.end(),[this](const string header) {
        this->add(header);
    });
}

// Implementation of copy constructor.
curl_header::curl_header(const curl_header &header) : headers(nullptr) {
    struct curl_slist *tmp_ptr = header.headers;
    while (tmp_ptr != nullptr) {
        this->add(tmp_ptr->data);
        tmp_ptr = tmp_ptr->next;
    }
}

// Implementation of assignment operator
curl_header &curl_header::operator=(const curl_header &header) {
    if (this == &header) {
        return *this;
    }
    struct curl_slist *tmp_ptr = header.headers;
    while (tmp_ptr != nullptr) {
        this->add(tmp_ptr->data);
        tmp_ptr = tmp_ptr->next;
    }
    return *this;
}

// Implementation of destructor.
curl_header::~curl_header() noexcept {
    if (this->headers != nullptr) {
        curl_slist_free_all(this->headers);
        this->headers = nullptr;
    }
}

// Implementation of add method.
void curl_header::add(const vector<string> &headers) {
    for_each(headers.begin(),headers.end(),[this](const string header) {
        this->add(header);
    });
}

// Implementation of add overloaded method.
void curl_header::add(const list<string> &headers) {
    for_each(headers.begin(),headers.end(),[this](const string header) {
        this->add(header);
    });
}

// Implementation of add overloaded method.
void curl_header::add(const string header) {
    this->headers = curl_slist_append(this->headers,header.c_str());
    if (this->headers == nullptr) {
        throw curl_error("*** Error while adding the header: "+header,__FUNCTION__);
    }
    ++this->size;
}

// Implementation of get method.
const struct curl_slist *curl_header::get() const {
    return this->headers;
}