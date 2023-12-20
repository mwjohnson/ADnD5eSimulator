
#include <iostream>
#include "characterFactory.h"
#include <curl/curl.h>

#include <json.h>
#include <json_object.h>




using namespace std;


int dowork() {
	CURL *curl;
	CURLcode res;

	static const char *postthis = "moo mooo moo moo";

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/character/char.php");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);

		/* if we don't provide POSTFIELDSIZE, libcurl will strlen() by
		itself */
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	return 0;
}
static std::string *DownloadedResponse;

static int writer(char *data, size_t size, size_t nmemb, std::string *buffer_in)
{

	// Is there anything in the buffer?  
	if (buffer_in != NULL)
	{
		// Append the data to the buffer    
		buffer_in->append(data, size * nmemb);

		// How much did we write?   
		DownloadedResponse = buffer_in;

		return static_cast<int>(size * nmemb);
	}

	return 0;

}

std::string DownloadJSON(std::string URL)
{
	CURL *curl;
	CURLcode res;
	struct curl_slist *headers = NULL; // init to NULL is important 

	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	headers = curl_slist_append(headers, "charsets: utf-8");
	curl = curl_easy_init();

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		res = curl_easy_perform(curl);

		if (CURLE_OK == res)
		{
			char *ct;
			res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
			if ((CURLE_OK == res) && ct)
				return *DownloadedResponse;
		}
	}

	curl_slist_free_all(headers);
}

json_object* makeJsonObjectArray() {
	json_object* my_array;

	my_array = json_object_new_array();
	json_object_array_add(my_array, json_object_new_int(1));
	json_object_array_add(my_array, json_object_new_int(2));
	json_object_array_add(my_array, json_object_new_int(3));
	json_object_array_put_idx(my_array, 4, json_object_new_int(5));
	json_object_array_put_idx(my_array, 3, json_object_new_int(4));
	json_object_array_put_idx(my_array, 6, json_object_new_int(7));

	return my_array;
}

/* holder for curl fetch */
struct curl_fetch_st {
	char *payload;
	size_t size;
};

/* callback for curl fetch */
size_t curl_callback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;                             /* calculate buffer size */
	struct curl_fetch_st *p = (struct curl_fetch_st *) userp;   /* cast pointer to fetch struct */

																/* expand buffer */
	p->payload = (char *)realloc(p->payload, p->size + realsize + 1);

	/* check buffer */
	if (p->payload == NULL) {
		/* this isn't good */
		fprintf(stderr, "ERROR: Failed to expand buffer in curl_callback");
		/* free buffer */
		free(p->payload);
		/* return */
		return -1;
	}

	/* copy contents to buffer */
	memcpy(&(p->payload[p->size]), contents, realsize);

	/* set new buffer size */
	p->size += realsize;

	/* ensure null termination */
	p->payload[p->size] = 0;

	/* return size */
	return realsize;
}

/* fetch and return url body via curl */
CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_st *fetch) {
	CURLcode rcode;                   /* curl result code */

									  /* init payload */
	fetch->payload = (char *)calloc(1, sizeof(fetch->payload));

	/* check payload */
	if (fetch->payload == NULL) {
		/* log error */
		fprintf(stderr, "ERROR: Failed to allocate payload in curl_fetch_url");
		/* return error */
		return CURLE_FAILED_INIT;
	}

	/* init size */
	fetch->size = 0;

	/* set url to fetch */
	curl_easy_setopt(ch, CURLOPT_URL, url);

	/* set calback function */
	curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, curl_callback);

	/* pass fetch struct pointer */
	curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *)fetch);

	/* set default user agent */
	curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	/* set timeout */
	curl_easy_setopt(ch, CURLOPT_TIMEOUT, 5);

	/* enable location redirects */
	curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);

	/* set maximum allowed redirects */
	curl_easy_setopt(ch, CURLOPT_MAXREDIRS, 1);

	/* fetch the url */
	rcode = curl_easy_perform(ch);

	/* return */
	return rcode;
}

int x() {
	CURL *ch;                                               /* curl handle */
	CURLcode rcode;                                         /* curl result code */

	json_object *json;                                      /* json post body */
	enum json_tokener_error jerr = json_tokener_success;    /* json parse error */

	struct curl_fetch_st curl_fetch;                        /* curl fetch struct */
	struct curl_fetch_st *cf = &curl_fetch;                 /* pointer to fetch struct */
	struct curl_slist *headers = NULL;                      /* http headers to send with request */

															/* url to test site */
	char *url = "http://jsonplaceholder.typicode.com/posts/";

	/* init curl handle */
	if ((ch = curl_easy_init()) == NULL) {
		/* log error */
		fprintf(stderr, "ERROR: Failed to create curl handle in fetch_session");
		/* return error */
		return 1;
	}

	/* set content type */
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");

	json = json_object_new_object();

	/* build post data */
	json_object_object_add(json, "title", json_object_new_string("testies"));
	json_object_object_add(json, "body", json_object_new_string("testies ... testies ... 1,2,3"));
	json_object_object_add(json, "userId", json_object_new_int(133));

	curl_easy_setopt(ch, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(ch, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(ch, CURLOPT_POSTFIELDS, json_object_to_json_string(json));

	/* fetch page and capture return code */
	rcode = curl_fetch_url(ch, url, cf);
	return 0;
}

void hack() {

	json_object* jo = makeJsonObjectArray();
	x();
	const char* xt = json_object_to_json_string(jo);
	std::cout << xt << "\n";


	json_object* json = json_object_new_object();

	/* build post data */
	json_object_object_add(json, "title", json_object_new_string("testies"));
	json_object_object_add(json, "body", json_object_new_string("testies ... testies ... 1,2,3"));
	json_object_object_add(json, "userId", json_object_new_int(133));

	const char* xjs = json_object_to_json_string(json);
	std::cout << xjs;

	json_object* jarray = json_object_new_array();

	json_object_array_add(jarray, json);
	json_object_array_add(jarray, json);
	json_object_array_add(jarray, json);

	const char* xja = json_object_to_json_string(jarray);
	std::cout << xja;

	//static_assert(decltype(xt)::dummy_error, "DUMP MY TYPE");

	//dowork();
	std::string char_url = "http://localhost/character/char.php?data=";
	char_url += xja;
	std::cout << char_url.c_str();
	//DownloadJSON(char_url);
}