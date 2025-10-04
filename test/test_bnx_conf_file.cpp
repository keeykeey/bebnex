#include <CppUTest/CommandLineTestRunner.h>
#include "core/bnx_conf_file.h"
#include "cassert"

TEST_GROUP(BnxReadToken){};
TEST(BnxReadToken, read_token_space)
{
    FILE *fp;
    fp = tmpfile();
    if (fp == nullptr) {
        CHECK_TEXT(false, "tmpfile() failed\n");
    }
    fprintf(fp, "listen 80");
    fseek(fp, 0, SEEK_SET);

    char buf[16] = {0};
    bnx_code_e ret = bnx_read_token(fp, buf, sizeof(buf));

    CHECK_EQUAL(BNX_OK, ret);
    STRCMP_EQUAL("listen", buf);
}

TEST(BnxReadToken, read_token_new_line)
{
    FILE *fp;
    fp = tmpfile();
    if (fp == nullptr) {
        CHECK_TEXT(false, "tmpfile() failed\n");
    }
    fprintf(fp, "listen\n80");
    fseek(fp, 0, SEEK_SET);

    char buf[16] = {0};
    bnx_code_e ret = bnx_read_token(fp, buf, sizeof(buf));

    CHECK_EQUAL(BNX_OK, ret);
    STRCMP_EQUAL("listen", buf);
}

TEST(BnxReadToken, read_token_tab)
{
    FILE *fp;
    fp = tmpfile();
    if (fp == nullptr) {
        CHECK_TEXT(false, "tmpfile() failed\n");
    }
    fprintf(fp, "listen\t80");
    fseek(fp, 0, SEEK_SET);

    char buf[16] = {0};
    bnx_code_e ret = bnx_read_token(fp, buf, sizeof(buf));

    CHECK_EQUAL(BNX_OK, ret);
    STRCMP_EQUAL("listen", buf);
}

TEST(BnxReadToken, read_token_eof)
{
    FILE *fp;
    fp = tmpfile();
    if (fp == nullptr) {
        CHECK_TEXT(false, "tmpfile() failed\n");
    }
    fprintf(fp, "listen");
    fseek(fp, 0, SEEK_SET);

    char buf[16] = {0};
    bnx_code_e ret = bnx_read_token(fp, buf, sizeof(buf));

    CHECK_EQUAL(BNX_EOF, ret);
    STRCMP_EQUAL("listen", buf);
}

TEST(BnxReadToken, read_token_max_length)
{
    FILE *fp;
    fp = tmpfile();
    if (fp == nullptr) {
        CHECK_TEXT(false, "tmpfile() failed\n");
    }
    fprintf(fp, "listen 80");
    fseek(fp, 0, SEEK_SET);

    char buf[16] = {0};
    bnx_code_e ret = bnx_read_token(fp, buf, 4);

    CHECK_EQUAL(BNX_STOP, ret);
    STRCMP_EQUAL("lis", buf);
}

TEST(BnxReadToken, read_token_starts_with_space)
{
    FILE *fp;
    fp = tmpfile();
    if (fp == nullptr) {
        CHECK_TEXT(false, "tmpfile() failed\n");
    }
    fprintf(fp, "  hello world");
    fseek(fp, 0, SEEK_SET);

    char buf[16] = {0};
    bnx_code_e ret = bnx_read_token(fp, buf, sizeof(buf));

    CHECK_EQUAL(BNX_OK, ret);
    STRCMP_EQUAL("hello", buf);
}

TEST(BnxReadToken, read_token_consecutively)
{
    FILE *fp;
    fp = tmpfile();
    if (fp == nullptr) {
        CHECK_TEXT(false, "tmpfile() failed\n");
    }
    fprintf(fp, "The dog\neats\tfood  there");
    fseek(fp, 0, SEEK_SET);

    char expected[5][8] = {"The", "dog", "eats", "food", "there"};
    bnx_code_e res;
    for (int i = 0; i < 5; ++i) {
        char buf[8] = {};
        res = bnx_read_token(fp, buf, sizeof(buf));
        STRCMP_EQUAL(expected[i], buf);
    }
}

TEST(BnxReadToken, read_token_fail)
{
    FILE *fp;
    fp = tmpfile();
    if (fp == nullptr) {
        CHECK_TEXT(false, "tmpfile() failed\n");
    }
    fprintf(fp, "listen 80");
    fseek(fp, 0, SEEK_SET);

    char buf[16] = {0};
    bnx_code_e ret = bnx_read_token(fp, buf, 0);

    CHECK_EQUAL(BNX_INVALID_ARGUMENT, ret);
    STRCMP_EQUAL("", buf);
}


TEST_GROUP(BnxConfTokenKind) {};
TEST(BnxConfTokenKind, http_is_conf_key)
{
    const char *token = "http";
    bnx_conf_token_kind_e result = bnx_conf_token_kind(token);
    CHECK_EQUAL(BNX_CONF_KEY, result);
}

TEST(BnxConfTokenKind, https_is_conf_key)
{
    const char *token = "https";
    bnx_conf_token_kind_e result = bnx_conf_token_kind(token);
    CHECK_EQUAL(BNX_CONF_KEY, result);
}

TEST(BnxConfTokenKind, server_is_conf_key)
{
    const char *token = "server";
    bnx_conf_token_kind_e result = bnx_conf_token_kind(token);
    CHECK_EQUAL(BNX_CONF_KEY, result);
}

TEST(BnxConfTokenKind, listen_is_conf_key)
{
    const char *token = "listen";
    bnx_conf_token_kind_e result = bnx_conf_token_kind(token);
    CHECK_EQUAL(BNX_CONF_KEY, result);
}

TEST(BnxConfTokenKind, location_is_conf_key)
{
    const char *token = "location";
    bnx_conf_token_kind_e result = bnx_conf_token_kind(token);
    CHECK_EQUAL(BNX_CONF_KEY, result);
}

TEST(BnxConfTokenKind, left_bracket_is_reserved_word)
{
    const char *token = "{";
    bnx_conf_token_kind_e result = bnx_conf_token_kind(token);
    CHECK_EQUAL(BNX_CONF_RESERVED, result);
}

TEST(BnxConfTokenKind, right_bracket_is_reserved_word)
{
    const char *token = "}";
    bnx_conf_token_kind_e result = bnx_conf_token_kind(token);
    CHECK_EQUAL(BNX_CONF_RESERVED, result);
}

TEST(BnxConfTokenKind, space_is_resersed_word)
{
    const char *token = " ";
    bnx_conf_token_kind_e result = bnx_conf_token_kind(token);
    CHECK_EQUAL(BNX_CONF_RESERVED, result);
}

TEST(BnxConfTokenKind, next_line_is_resersed_word)
{
    const char *token = "\n";
    bnx_conf_token_kind_e result = bnx_conf_token_kind(token);
    CHECK_EQUAL(BNX_CONF_RESERVED, result);
}

TEST(BnxConfTokenKind, execute_with_invalid_argument)
{
    bnx_conf_token_kind_e result = bnx_conf_token_kind(NULL);
    CHECK_EQUAL(BNX_CONF_NO_MATCH, result);
}


TEST_GROUP(BnxConfAllowedKey) {};
TEST(BnxConfAllowedKey, find_match_key)
{
    const char *allowed_keys[] = { "apple", "banana", "http", "grape" };
    bnx_code_e result = bnx_conf_allowed_key("http", allowed_keys, 4);
    CHECK_EQUAL(BNX_OK, result);
}

TEST(BnxConfAllowedKey, find_no_match_key)
{
    const char *allowed_keys[] = { "apple", "banana", "orange", "grape" };
    bnx_code_e result = bnx_conf_allowed_key("http", allowed_keys, 4);
    CHECK_EQUAL(BNX_ERROR, result);
}

TEST(BnxConfAllowedKey, find_http_success)
{
    size_t array_count = sizeof(bnx_allowed_conf_key_array) / sizeof(bnx_allowed_conf_key_array[0]);
    bnx_code_e result = bnx_conf_allowed_key("http", bnx_allowed_conf_key_array, array_count);
    CHECK_EQUAL(BNX_OK, result);
}

TEST(BnxConfAllowedKey, allowed_conf_key_definition_is_valid)
{
    size_t array_count = sizeof(bnx_allowed_conf_key_array) / sizeof(bnx_allowed_conf_key_array[0]);
    bnx_code_e result = bnx_conf_allowed_key("https", bnx_allowed_conf_key_array, array_count);
    CHECK_EQUAL(BNX_OK, result);
    result = bnx_conf_allowed_key("server", bnx_allowed_conf_key_array, array_count);
    CHECK_EQUAL(BNX_OK, result);
    result = bnx_conf_allowed_key("listen", bnx_allowed_conf_key_array, array_count);
    CHECK_EQUAL(BNX_OK, result);
    result = bnx_conf_allowed_key("location", bnx_allowed_conf_key_array, array_count);
    CHECK_EQUAL(BNX_OK, result);
}

TEST(BnxConfAllowedKey, execute_with_invalid_argument)
{
    size_t array_count = sizeof(bnx_allowed_conf_key_array) / sizeof(bnx_allowed_conf_key_array[0]);
    bnx_code_e result = bnx_conf_allowed_key(NULL, bnx_allowed_conf_key_array, array_count);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
}

TEST(BnxConfAllowedKey, execute_with_next_line)
{
    size_t array_count = sizeof(bnx_allowed_conf_key_array) / sizeof(bnx_allowed_conf_key_array[0]);
    bnx_code_e result = bnx_conf_allowed_key("\n", bnx_allowed_conf_key_array, array_count);
    CHECK_EQUAL(BNX_ERROR, result);
}

TEST(BnxConfAllowedKey, execute_with_space)
{
    size_t array_count = sizeof(bnx_allowed_conf_key_array) / sizeof(bnx_allowed_conf_key_array[0]);
    bnx_code_e result = bnx_conf_allowed_key(" ", bnx_allowed_conf_key_array, array_count);
    CHECK_EQUAL(BNX_ERROR, result);
}


TEST_GROUP(BnxConfReservedToken) {};
TEST(BnxConfReservedToken, match_reserved_token)
{
    const char *reserved_token[] = {
        "$", "%", "^", "{", "}", " ", "\n", NULL
    };

    size_t arr_size = sizeof(reserved_token)/sizeof(reserved_token[0]);
    bnx_code_e result = bnx_conf_reserved_token("{", reserved_token, 8);
    CHECK_EQUAL(BNX_OK, result);
}

TEST(BnxConfReservedToken, match_reserved_token_at_last)
{
    const char *reserved_token[] = {
        "$", "%", "^", "{", "}", " ", "\n", NULL
    };

    size_t arr_size = sizeof(reserved_token)/sizeof(reserved_token[0]);
    bnx_code_e result = bnx_conf_reserved_token("\n", reserved_token, 8);
    CHECK_EQUAL(BNX_OK, result);
}

TEST(BnxConfReservedToken, match_fail)
{
    const char *reserved_token[] = {
        "$", "%", "^", "{", "}", " ", "\n", NULL
    };

    size_t arr_size = sizeof(reserved_token)/sizeof(reserved_token[0]);
    bnx_code_e result = bnx_conf_reserved_token("+", reserved_token, 8);
    CHECK_EQUAL(BNX_ERROR, result);
}

TEST(BnxConfReservedToken, execute_with_invalid_argument)
{
    const char *reserved_token[] = {
        "$", "%", "^", "{", "}", " ", "\n", NULL
    };

    size_t arr_size = sizeof(reserved_token)/sizeof(reserved_token[0]);
    bnx_code_e result = bnx_conf_reserved_token(NULL, reserved_token, 8);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
}


TEST_GROUP(BnxConfInit){};
TEST(BnxConfInit, init_conf_success)
{
    bnx_conf_t *conf = NULL;
    bnx_code_e res = bnx_conf_init(&conf, "http", 4);
    CHECK_EQUAL(BNX_OK, res);
    POINTERS_EQUAL(NULL, conf->parent);
    STRCMP_EQUAL("http", conf->key);
    POINTERS_EQUAL(NULL, conf->value);
    POINTERS_EQUAL(NULL, conf->children);
    CHECK_EQUAL(0, conf->children_count);
    
    res = bnx_conf_free(&conf);
    CHECK_EQUAL(BNX_OK, res);
}

TEST(BnxConfInit, init_conf_with_just_key_length)
{
    assert(BNX_CONF_KEY_MAX_LENGTH == 16);
    bnx_conf_t *conf = NULL;
    bnx_code_e res = bnx_conf_init(&conf, "0123456789abcdef", 16);
    CHECK_EQUAL(BNX_OK, res);

    POINTERS_EQUAL(NULL, conf->parent);
    STRCMP_EQUAL("0123456789abcde", conf->key);
    POINTERS_EQUAL(NULL, conf->value);
    POINTERS_EQUAL(NULL, conf->children);
    CHECK_EQUAL(0, conf->children_count);
    
    res = bnx_conf_free(&conf);
    CHECK_EQUAL(BNX_OK, res);
}

TEST(BnxConfInit, init_conf_with_too_long_key)
{
    assert(BNX_CONF_KEY_MAX_LENGTH == 16);
    bnx_conf_t *conf = NULL;
    bnx_code_e res = bnx_conf_init(&conf, "12345678901234567890", 20);
    CHECK_EQUAL(BNX_OK, res);

    POINTERS_EQUAL(NULL, conf->parent);
    STRCMP_EQUAL("123456789012345", conf->key);
    POINTERS_EQUAL(NULL, conf->value);
    POINTERS_EQUAL(NULL, conf->children);
    CHECK_EQUAL(0, conf->children_count);
    
    res = bnx_conf_free(&conf);
    CHECK_EQUAL(BNX_OK, res);
}


TEST_GROUP(BnxConfFree) {};
TEST(BnxConfFree, free_nullptr)
{
    bnx_code_e res = bnx_conf_free(NULL);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, res);
}

TEST(BnxConfFree, free_children)
{
    bnx_conf_t *root = NULL;
    bnx_conf_t *http = NULL;
    bnx_conf_t *https = NULL;
    bnx_conf_t *server = NULL;
    bnx_conf_init(&root, "root", 4);
    bnx_conf_init(&http, "http", 4);
    bnx_conf_init(&https, "https", 5);
    bnx_conf_init(&server, "server", 6);

    bnx_conf_add_child(root, http);
    bnx_conf_add_child(root, https);
    bnx_conf_add_child(http, server);

    POINTERS_EQUAL(http, root->children[0]);
    POINTERS_EQUAL(https, root->children[1]);
    bnx_conf_free(&root);

    POINTERS_EQUAL(NULL, root);
}


TEST_GROUP(BnxConfAddChildren) {};
TEST(BnxConfAddChildren, add_first_child)
{
    bnx_conf_t *http = NULL;
    bnx_conf_t *server = NULL;
    bnx_conf_init(&http, "http", 4);
    bnx_conf_init(&server, "server", 6);

    bnx_code_e result = bnx_conf_add_child(http, server);
    // check return value
    CHECK_EQUAL(BNX_OK, result);
    // check http
    POINTERS_EQUAL(NULL, http->parent);
    STRCMP_EQUAL("http", http->key);
    POINTERS_EQUAL(NULL, http->value);
    CHECK_EQUAL(0, http->value_count);
    POINTERS_EQUAL(server, http->children[0]);
    STRCMP_EQUAL(server->key, http->children[0]->key);
    CHECK_EQUAL(1, http->children_count);
    // check server
    POINTERS_EQUAL(http, server->parent);
    STRCMP_EQUAL("server", server->key);
    POINTERS_EQUAL(NULL, server->value);
    CHECK_EQUAL(0, server->value_count);
    POINTERS_EQUAL(NULL, server->children);
    CHECK_EQUAL(0, server->children_count);

    bnx_conf_free(&http);
}

TEST(BnxConfAddChildren, add_two_children)
{
    bnx_conf_t *root = NULL;
    bnx_conf_t *http = NULL;
    bnx_conf_t *https = NULL;
    bnx_conf_init(&root, "root", 4);
    bnx_conf_init(&http, "http", 4);
    bnx_conf_init(&https, "https", 5);

    bnx_conf_add_child(root, http);
    bnx_conf_add_child(root, https);

    // check root's
    POINTERS_EQUAL(NULL, root->parent);
    STRCMP_EQUAL("root", root->key);
    POINTERS_EQUAL(NULL, root->value);
    CHECK_EQUAL(0, root->value_count);
    POINTERS_EQUAL(http, root->children[0]);
    STRCMP_EQUAL(http->key, root->children[0]->key);
    POINTERS_EQUAL(https, root->children[1]);
    STRCMP_EQUAL(https->key, root->children[1]->key);
    CHECK_EQUAL(2, root->children_count);

    // check http
    POINTERS_EQUAL(root, http->parent);
    STRCMP_EQUAL("http", http->key);
    POINTERS_EQUAL(NULL, http->value);
    CHECK_EQUAL(0, http->value_count);
    POINTERS_EQUAL(NULL, http->children);
    CHECK_EQUAL(0, http->children_count);

    // check https
    POINTERS_EQUAL(root, https->parent);
    STRCMP_EQUAL("https", https->key);
    POINTERS_EQUAL(NULL, https->value);
    CHECK_EQUAL(0, https->value_count);
    POINTERS_EQUAL(NULL, https->children);
    CHECK_EQUAL(0, https->children_count);

    bnx_conf_free(&root);
}

TEST(BnxConfAddChildren, add_grand_child)
{
    bnx_conf_t *root = NULL;
    bnx_conf_t *http = NULL;
    bnx_conf_t *server = NULL;

    bnx_conf_init(&root, "root", 4);
    bnx_conf_init(&http, "http", 4);
    bnx_conf_init(&server, "server", 6);

    bnx_conf_add_child(root, http);
    bnx_conf_add_child(http, server);

    // check root
    POINTERS_EQUAL(NULL, root->parent);
    STRCMP_EQUAL("root", root->key);
    POINTERS_EQUAL(NULL, root->value);
    CHECK_EQUAL(0, root->value_count);
    POINTERS_EQUAL(http, root->children[0]);
    CHECK_EQUAL(1, root->children_count);

    // check http
    POINTERS_EQUAL(root, http->parent);
    STRCMP_EQUAL("http", http->key);
    POINTERS_EQUAL(NULL, http->value);
    CHECK_EQUAL(0, http->value_count);
    POINTERS_EQUAL(server, http->children[0]);
    CHECK_EQUAL(1, http->children_count);

    // check server
    POINTERS_EQUAL(http, server->parent);
    STRCMP_EQUAL("server", server->key);
    POINTERS_EQUAL(NULL, server->value);
    CHECK_EQUAL(0, server->value_count);
    POINTERS_EQUAL(NULL, server->children);
    CHECK_EQUAL(0, server->children_count);

    bnx_conf_free(&root);
}

TEST(BnxConfAddChildren, add_same_child_twice_fail)
{
    bnx_conf_t *http = NULL;
    bnx_conf_t *https = NULL;
    bnx_conf_t *server = NULL;

    bnx_conf_init(&http, "http", 4);
    bnx_conf_init(&https, "https", 5);
    bnx_conf_init(&server, "server", 6);

    bnx_code_e result = bnx_conf_add_child(http, server);
    CHECK_EQUAL(BNX_OK, result);

    result = bnx_conf_add_child(https, server);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
    POINTERS_EQUAL(NULL, https->children);
    CHECK_EQUAL(0, https->children_count);
    CHECK_EQUAL(http, server->parent);
    CHECK_EQUAL(server, http->children[0]);
}


TEST_GROUP(BnxConfAddValue) {};
TEST(BnxConfAddValue, add_one_value)
{
    bnx_code_e code;
    bnx_conf_t *fruits = NULL;
    bnx_conf_init(&fruits, "fruits", 6);
    code = bnx_conf_add_value(fruits, "apple");
    STRCMP_EQUAL("apple", fruits->value[0]);
    CHECK_EQUAL(1, fruits->value_count);
    CHECK_EQUAL(BNX_OK, code);
}

TEST(BnxConfAddValue, add_some_value)
{
    bnx_conf_t *fruits = NULL;
    bnx_conf_init(&fruits, "fruits", 6);
    bnx_conf_add_value(fruits, "apple");
    bnx_conf_add_value(fruits, "banana");
    bnx_conf_add_value(fruits, "orange");
    STRCMP_EQUAL("apple", fruits->value[0]);
    STRCMP_EQUAL("banana", fruits->value[1]);
    STRCMP_EQUAL("orange", fruits->value[2]);
    CHECK_EQUAL(3, fruits->value_count);
}

TEST(BnxConfAddValue, execute_with_invalid_argument)
{
    bnx_code_e result;
    result = bnx_conf_add_value(NULL, "foo");
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
}


TEST_GROUP(BnxConfValidPair) {
    const bnx_conf_pair_t conf_pair[6] = {
        { "root", "http" },
        { "root", "https" },
        { "http", "server" },
        { "https", "server" },
        { "server", "listen" },
        { "server", "location" },
    };
    size_t size = sizeof(conf_pair)/sizeof(conf_pair[0]);
};

TEST(BnxConfValidPair, parse_succcess)
{
    bnx_conf_t *root = NULL;
    bnx_conf_t *http = NULL;
    bnx_conf_init(&root, "server", sizeof("server"));
    bnx_conf_init(&http, "listen", sizeof("listen"));

    bnx_code_e result = bnx_conf_valid_pair(root, http, conf_pair, size);
    CHECK_EQUAL(BNX_OK, result);

    bnx_conf_free(&root);
}

TEST(BnxConfValidPair, parse_succcess_with_pointer_pointer)
{
    bnx_conf_t *root = NULL;
    bnx_conf_t *http = NULL;
    bnx_conf_t **current = &root;
    bnx_conf_init(current, "server", sizeof("server"));
    bnx_conf_init(&http, "listen", sizeof("listen"));

    bnx_code_e result = bnx_conf_valid_pair(root, http, conf_pair, size);
    CHECK_EQUAL(BNX_OK, result);

    bnx_conf_free(&root);
}

TEST(BnxConfValidPair, parse_fail)
{
    bnx_conf_t *hoge = NULL;
    bnx_conf_t *http = NULL;
    bnx_conf_init(&hoge, "hoge", 4);
    bnx_conf_init(&http, "http", sizeof("http"));

    bnx_code_e result = bnx_conf_valid_pair(hoge, http, conf_pair, size);
    CHECK_EQUAL(BNX_ERROR, result);

    bnx_conf_free(&hoge);
    bnx_conf_free(&http);
}

TEST(BnxConfValidPair, parse_invalid_argument_both)
{
    bnx_conf_t *parent = NULL;
    bnx_conf_t *child = NULL;

    bnx_code_e result = bnx_conf_valid_pair(parent, child, conf_pair, size);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);
}

TEST(BnxConfValidPair, parse_invalid_argument_parent)
{
    bnx_conf_t *parent = NULL;
    bnx_conf_t *child = NULL;
    bnx_conf_init(&child, "server", 4);

    bnx_code_e result = bnx_conf_valid_pair(parent, child, conf_pair, size);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);

    bnx_conf_free(&child);
}

TEST(BnxConfValidPair, parse_invalid_argument_child)
{
    bnx_conf_t *parent = NULL;
    bnx_conf_t *child = NULL;
    bnx_conf_init(&parent, "https", 4);

    bnx_code_e result = bnx_conf_valid_pair(parent, child, conf_pair, size);
    CHECK_EQUAL(BNX_INVALID_ARGUMENT, result);

    bnx_conf_free(&parent);
}


TEST_GROUP(BnxConfRead) {};
TEST(BnxConfRead, read_one_http_block_success)
{
    bnx_conf *root = NULL;
    bnx_conf_init(&root, "root", 4);

    FILE *fp = tmpfile();
    if (fp == NULL) CHECK_TEXT(false, "tmpfile() failed");
    fprintf(fp, "http {\n server { listen 80 }\n}\n");
    fseek(fp, 0, SEEK_SET);

    bnx_code_e result = bnx_conf_read(root, fp);
    CHECK_EQUAL(BNX_EOF, result);
    // http
    CHECK_EQUAL(1, root->children_count);
    STRCMP_EQUAL("http", root->children[0]->key);
    bnx_conf_t *http = root->children[0];
    POINTERS_EQUAL(root, http->parent);
    // server
    CHECK_EQUAL(1, http->children_count);
    STRCMP_EQUAL("server", http->children[0]->key);
    bnx_conf_t *server = http->children[0];
    // listen
    CHECK_EQUAL(1, server->children_count);
    STRCMP_EQUAL("listen", server->children[0]->key);
    bnx_conf_t *listen = server->children[0];

    CHECK_EQUAL(1, listen->value_count);
    STRCMP_EQUAL("80", listen->value[0]);

    fclose(fp);
    bnx_conf_free(&root);
}

TEST(BnxConfRead, read_tow_brother_block_success)
{
    bnx_conf *root = NULL;
    bnx_conf_init(&root, "root", 4);

    FILE *fp = tmpfile();
    if (fp == NULL) CHECK_TEXT(false, "tmpfile() failed");
    fprintf(fp, "http {\n server { listen 80 }\n}\n https {\n server { listen 443 }\n}\n");
    fseek(fp, 0, SEEK_SET);

   
    bnx_code_e result = bnx_conf_read(root, fp);
    CHECK_EQUAL(BNX_EOF, result);

    // http
    CHECK_EQUAL(2, root->children_count);
    STRCMP_EQUAL("http", root->children[0]->key);
    bnx_conf_t *http = root->children[0];
    POINTERS_EQUAL(root, http->parent);
    // server
    CHECK_EQUAL(1, http->children_count);
    STRCMP_EQUAL("server", http->children[0]->key);
    bnx_conf_t *http_server = http->children[0];
    // listen
    CHECK_EQUAL(1, http_server->children_count);
    STRCMP_EQUAL("listen", http_server->children[0]->key);
    bnx_conf_t *http_listen = root->children[0]->children[0]->children[0];

    CHECK_EQUAL(1, http_listen->value_count);
    STRCMP_EQUAL("80", http_listen->value[0]);


    // https
    CHECK_EQUAL(2, root->children_count);
    STRCMP_EQUAL("https", root->children[1]->key);
    bnx_conf_t *https = root->children[1];
    // server
    CHECK_EQUAL(1, https->children_count);
    STRCMP_EQUAL("server", https->children[0]->key);
    bnx_conf_t *https_server = https->children[0];
    // listen
    CHECK_EQUAL(1, https_server->children_count);
    STRCMP_EQUAL("listen", http_server->children[0]->key);
    bnx_conf_t *https_listen = https_server->children[0];

    CHECK_EQUAL(1, https_listen->value_count);
    STRCMP_EQUAL("443", https_listen->value[0]);

    fclose(fp);
    bnx_conf_free(&root);
}

TEST(BnxConfRead, read_http_and_https_block_success)
{
    bnx_conf *root = NULL;
    bnx_conf_init(&root, "root", 4);

    FILE *fp = tmpfile();
    if (fp == NULL) CHECK_TEXT(false, "tmpfile() failed");
    fprintf(fp, "\
http {\n\
  server {\
    listen 80\
  }\n\
}\n\
\
https {\n\
  server {\
    listen 443\
  }\n\
}\n\
");
    fseek(fp, 0, SEEK_SET);

    bnx_code_e result = bnx_conf_read(root, fp);
    CHECK_EQUAL(BNX_EOF, result);

    // http
    CHECK_EQUAL(2, root->children_count);
    STRCMP_EQUAL("http", root->children[0]->key);
    bnx_conf_t *http = root->children[0];
    POINTERS_EQUAL(root, http->parent);
    // server
    CHECK_EQUAL(1, http->children_count);
    STRCMP_EQUAL("server", http->children[0]->key);
    bnx_conf_t *http_server = http->children[0];
    // listen
    CHECK_EQUAL(1, http_server->children_count);
    STRCMP_EQUAL("listen", http_server->children[0]->key);
    bnx_conf_t *http_listen = root->children[0]->children[0]->children[0];

    CHECK_EQUAL(1, http_listen->value_count);
    STRCMP_EQUAL("80", http_listen->value[0]);


    // https
    CHECK_EQUAL(2, root->children_count);
    STRCMP_EQUAL("https", root->children[1]->key);
    bnx_conf_t *https = root->children[1];
    // server
    CHECK_EQUAL(1, https->children_count);
    STRCMP_EQUAL("server", https->children[0]->key);
    bnx_conf_t *https_server = https->children[0];
    // listen
    CHECK_EQUAL(1, https_server->children_count);
    STRCMP_EQUAL("listen", http_server->children[0]->key);
    bnx_conf_t *https_listen = https_server->children[0];

    CHECK_EQUAL(1, https_listen->value_count);
    STRCMP_EQUAL("443", https_listen->value[0]);

    fclose(fp);
    bnx_conf_free(&root);
}

TEST(BnxConfRead, read_fail_with_block_end_without_block_start)
{
    bnx_conf *root = NULL;
    bnx_conf_init(&root, "root", 4);

    FILE *fp = tmpfile();
    if (fp == NULL) CHECK_TEXT(false, "tmpfile() failed");
    // server block is ending with } but it isn't starting with {
    fprintf(fp, "http{server }\n");
    fseek(fp, 0, SEEK_SET);

    bnx_code_e result = bnx_conf_read(root, fp);
    CHECK_EQUAL(BNX_ERROR, result);

    fclose(fp);
    bnx_conf_free(&root);
}


TEST_GROUP(BnxConfCheckValidFile) {};

TEST(BnxConfCheckValidFile, check_success)
{
    FILE *fp = tmpfile();
    if (fp == NULL) CHECK_TEXT(false, "tmpfile() failed");

    fprintf(fp, "http { server { listen 80 } }\n");
    fseek(fp, 0, SEEK_SET);

    bnx_code_e result = bnx_conf_check_valid_file(fp);
    CHECK_EQUAL(BNX_OK, result);
    fclose(fp);
}

TEST(BnxConfCheckValidFile, check_file_end_with_next_line)
{
    FILE *fp = tmpfile();
    if (fp == NULL) CHECK_TEXT(false, "tmpfile() failed");

    fprintf(fp, "bebnex config file is expected to end with next line");
    fseek(fp, 0, SEEK_SET);

    bnx_code_e result = bnx_conf_check_valid_file(fp);
    CHECK_EQUAL(BNX_ERROR, result);
    fclose(fp);
}

TEST(BnxConfCheckValidFile, ensure_same_number_of_bracket_start_and_end)
{
    FILE *fp = tmpfile();
    if (fp == NULL) CHECK_TEXT(false, "tmpfile() failed");

    fprintf(fp, "{ raise ERROR if number of bracket start { { != number of bracket end } }\n");
    fseek(fp, 0, SEEK_SET);

    bnx_code_e result = bnx_conf_check_valid_file(fp);
    CHECK_EQUAL(BNX_ERROR, result);
    fclose(fp);
}


TEST(BnxConfCheckValidFile, ensure_bracket_is_after_spaces)
{
    FILE *fp = tmpfile();
    if (fp == NULL) CHECK_TEXT(false, "tmpfile() failed");

    fprintf(fp, "http{ server\n");
    fseek(fp, 0, SEEK_SET);

    bnx_code_e result = bnx_conf_check_valid_file(fp);
    CHECK_EQUAL(BNX_ERROR, result);
    fclose(fp);
}

TEST(BnxConfCheckValidFile, ensure_bracket_is_before_spaces)
{
    FILE *fp = tmpfile();
    if (fp == NULL) CHECK_TEXT(false, "tmpfile() failed");

    fprintf(fp, "http {server\n");
    fseek(fp, 0, SEEK_SET);

    bnx_code_e result = bnx_conf_check_valid_file(fp);
    CHECK_EQUAL(BNX_ERROR, result);
    fclose(fp);
}

TEST(BnxConfCheckValidFile, ensure_end_with_next_line)
{
    FILE *fp = tmpfile();
    if (fp == NULL) CHECK_TEXT(false, "tmpfile() failed");

    fprintf(fp, "http { server { listen 80 } }");
    fseek(fp, 0, SEEK_SET);

    bnx_code_e result = bnx_conf_check_valid_file(fp);
    CHECK_EQUAL(BNX_ERROR, result);
    fclose(fp);
}
