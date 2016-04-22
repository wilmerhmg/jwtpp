/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Artur Troian
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include <errno.h>

#include <json/json.h>

#include <tools/tools.hpp>

/**
 * \brief
 */
typedef enum jwt_alg {
	JWT_ALG_NONE = 0,
	JWT_ALG_HS256,
	JWT_ALG_HS384,
	JWT_ALG_HS512,
	JWT_ALG_UKNOWN
} jwt_alg_t;

/**
 * \brief
 */
class jwt final {
public:
	/**
	 * \brief  Constructor to sign token
	 *
	 * \param[in]  alg: Algorithm type to use for signature
	 * \param[in]  key: secret to sign
	 * \param[in]  len: key size
	 */
	explicit jwt(jwt_alg_t alg);

	/**
	 * \brief  Constructor to verify token
	 *
	 * \param[in]  token: token in format header.payload.signature to verify
	 * \param[in]  key: secret to check signature
	 * \param[in]  len: key size
	 */
	explicit jwt(const std::string &token);

	~jwt();

	/**
	 * \brief
	 *
	 * \param[in]  key:
	 * \param[in]  value:
	 *
	 * \return None
	 */
	void add_grant(const std::string &key, const std::string &value);

	/**
	 * \brief  Verify grant value
	 *
	 * \param[in]  grant:
	 * \param[in]  value: expected value
	 *
	 * \retval  true: verified
	 * \retval  false: not verified
	 * \throws  std::invalid_argument("Invalid params")
	 * \throws  std::runtime_error("Grant not found")
	 */
	bool grant_verify(const std::string &key, const std::string &value);

	/**
	 * \brief
	 *
	 * \param[in]
	 * \param[in]
	 *
	 * \return
	 */
	std::string sign(const uint8_t *key, size_t key_size);

	/**
	 * \brief
	 *
	 * \retval  true: signature verified
	 * \retval  false: bad signature
	 */
	bool verify(const uint8_t *key, size_t size);

	void cleanup();

private:
	/**
	 * \brief
	 *
	 * \param[in]
	 */
	void parse(const std::string &token);

	/**
	 * \brief
	 *
	 * \param[in]
	 * \param[in]
	 *
	 * \return
	 */
	std::vector<std::string> split(const std::string &text, char sep);

	/**
	 * \brief   Sign data with key
	 *
	 * \param[out]  signature:
	 * \param[in]   data: data to be signed
	 *
	 * \return
	 */
	int gen_signature(std::string &signature, const std::string &data, const uint8_t *key, size_t key_size);

private:
	/**
	 * \brief
	 *
	 * \param[in]
	 * \param[in]
	 *
	 * \return None
	 */
	static void base64uri_encode(uint8_t *buf, size_t len);

	/**
	 * \brief
	 *
	 * \param[in]  str:
	 *
	 * \return None
	 */
	static void base64uri_encode(std::string &str);

	/**
	 * \brief  Calculate length of key for given algorithm
	 *
	 * \param[in]  alg: value of type jwt_alg_t
	 *
	 * \retval  0 or positiv value: size of key
	 * \retval  -1: Unknown algorithm
	 */
	static int alg_key_len(jwt_alg_t alg);

	/**
	 * \brief  Convert algorithm represented as string into enum
	 *
	 * \param[in]  alg: Algorithm name as null-terminated string
	 *
	 * \retval     value of type jwt_alg_t
	 */
	static jwt_alg_t str2alg(const char *alg);

	/**
	 * brief  Convert algorithm value to it's string representation
	 *
	 * \param[in]  alg: value of type jwt_alg_t
	 *
	 * \retval     Null-terminated string representation or nullptr if value unknown
	 */
	static const char *alg2str(jwt_alg_t alg);

private:
	jwt_alg_t                m_alg;
	Json::Value              m_header;
	Json::Value              m_payload;
	std::string              m_signature;
	std::vector<std::string> m_tokens;
};