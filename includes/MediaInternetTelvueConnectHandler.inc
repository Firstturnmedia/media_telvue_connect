<?php

/**
 * @file media_telvue_connect/includes/MediaInterenetTelvueHandler.inc
 *
 * Contains MediaInternetTelvueConnectHandler.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetTelvueConnectHandler extends MediaInternetBaseHandler {
  /**
   * Check if a Telvue video id is valid.
   *
   * Check against the oembed stream instead of the gdata api site to
   * avoid "yt:quota too_many_recent_calls" errors.
   *
   * @return
   *   Boolean.
   */
  static public function validId($id) {
    return TRUE;
  }

  public function parse($embedCode) {
    $domains = array(
      'videoplayer.telvue.com/player',
    );

    // handle encoded url also
    $query = parse_url(html_entity_decode($embedCode));

    // handle RESTful url, extract id
    // e.g. https://videoplayer.telvue.com/player/BYZxzRoRv7IoaYynQYVU7BGYzrtXvUiD/media/286963?autostart=false&showtabssearch=true&fullscreen=false
    $matches = array();

    if ($query['host'] == 'videoplayer.telvue.com') {
      if (preg_match('#/media/(.*)#i', $query['path'], $matches)) {
        $video = $matches[1];
      }

      if (preg_match('#/player/(.*)#i', $query['path'], $matches)) {
        $match = explode('/', $matches[1]);
        $id = $match[0];
      }
    }

    $domain = $query['host'] . $query['path'];
    parse_str($query['query'], $params);

    foreach ($params as $key => $value) {
      switch ($key) {
        case 'id':
          $id = $value;
          break;
        case 'video':
          $video = $value;
          break;
        case 'width':
          $width = $value;
          break;
        case 'height':
          $height = $value;
          break;
      }
    }

    // preparing regex
    foreach ($domains as $key => $value) {
      $domains[$key] = preg_quote($value);
    }

    $regex = '#^' . implode('|', $domains) .'#i';

    if (preg_match($regex, $domain) && isset($id) && isset($video) && self::validId($id)) {
      $size = '';

      if (!isset($width) || !isset($width)) {
        unset($width);
        unset($height);
      }
      else {
        $size = '/width/' . $width . '/height/' . $height;
      }

      return file_stream_wrapper_uri_normalize('telvue_connect://id/' . $id . $size . '/video/' . $video);
    }
  }

  public function claim($embedCode) {
    if ($this->parse($embedCode)) {
      return TRUE;
    }
  }

  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri, TRUE);
    return $file;
  }

  /**
   * Returns information about the media. See http://video.search.yahoo.com/mrss.
   *
   * @return
   *   If ATOM+MRSS information is available, a SimpleXML element containing
   *   ATOM and MRSS elements, as per those respective specifications.
   *
   * @todo Would be better for the return value to be an array rather than a
   *   SimpleXML element, but media_retrieve_xml() needs to be upgraded to
   *   handle namespaces first.
   */
  public function getMRSS() {
    $uri = $this->parse($this->embedCode);
    $client_id = arg(1, file_uri_target($uri));
    $rss_url = url('http://videoplayer.telvue.com/channels/' . $client_id, array('query' => array('v' => '2')));
    // @todo Use media_retrieve_xml() once it's upgraded to include elements
    //   from all namespaces, not just the document default namespace.
    $request = drupal_http_request($rss_url);

    if (!isset($request->error)) {
      $entry = simplexml_load_string($request->data);
    }
    else {
      throw new Exception("Error Processing Request. (Error: {$response->code}, {$response->error})");

      //if request wasn't successful, create object for return to avoid errors
      $entry = new SimpleXMLElement();
    }
    
    return $entry;
  }

  /**
   * Returns information about the media. See http://www.oembed.com/.
   *
   * @return
   *   If oEmbed information is available, an array containing 'title', 'type',
   *   'url', and other information as specified by the oEmbed standard.
   *   Otherwise, NULL.
   */
  public function getOEmbed() {
    //no oembed support yet
  }
}
