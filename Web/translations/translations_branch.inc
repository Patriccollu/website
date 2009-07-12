<?php
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

  /**
   * ...
   *
   * Based partially on the TortoiseSVN Drupal module:
   * @link http://tortoisesvn.tigris.org/svn/tortoisesvn/trunk/contrib/drupal-modules/module/tortoisesvn.module
   */
  function get_status_graph($count, $translated, $fuzzy, $untranslated) {
    // Calculate width of bars
    $wc = 150;
    $wt = round($wc * $translated / $count);
    $wf = round($wc * $fuzzy / $count);
    $wu = round($wc * $untranslated / $count);
    
    // Make sure that each bar is at least 2px wide if its value is > 0
    if (($wt<2) && ($translated>0)) $wt=2;
    if (($wf<2) && ($fuzzy>0)) $wf=2;
    if (($wu<2) && ($untranslated>0)) $wu=2;
    
    // Make sure total width stays constant by determining the widest bar
    // and subtracting the width of the shorter bars from it.
    if ($wt+$wf+$wu!=$wc) {
      if ($wt>=$wf && $wt>=$wu) {
        $wt = $wc-$wf-$wu;
      } else if ($wf>=$wt && $wf>=$wu) {
        $wf = $wc-$wt-$wu;
      } else {
        $wu = $wc-$wt-$wf;
      }
    }
    
    // Calculate percentages
    $pt = ($translated * 100) / $count;
    $pf = ($fuzzy * 100) / $count;
    $pu = ($untranslated * 100) / $count;
    
    $temp = "";
    if ($wt > 0)
      $temp .= "<img src=\"images/translated.png\" alt=\"Translated\" title=\"Translated: " . round($pt, 1) . "%\" width=\"$wt\" height=\"16\">";
    if ($wf > 0)
      $temp .= "<img src=\"images/fuzzy.png\" alt=\"Fuzzy\" title=\"Fuzzy: " . round($pf, 1) . "%\" width=\"$wf\" height=\"16\">";
    if ($wu > 0)
      $temp .= "<img src=\"images/untranslated.png\" alt=\"Untranslated\" title=\"Untranslated: " . round($pu, 1) . "%\" width=\"$wu\" height=\"16\">";
    
    return $temp;
  }
  
  /**
   * ...
   */
  function printTranslationsStatus($xmlarray, $svnurl) {
    global $page;
    
    $update = $xmlarray['TRANSLATIONS'][0]['UPDATE'][0]['VALUE'];
    $temp = "<p>Status from <strong>" . $update . "</strong>:</p>\n";
    $temp .= "<table class=\"status\">\n";
    $temp .= "  <tr>\n";
    $temp .= "    <th class=\"left\">Language</th>\n";
    $temp .= "    <th class=\"center\">Graph</th>\n";
    $temp .= "    <th class=\"right translated\">Translated</th>\n";
    $temp .= "    <th class=\"right fuzzy\">Fuzzy</th>\n";
    $temp .= "    <th class=\"right untranslated\">Untranslated</th>\n";
    $temp .= "    <th class=\"center\">Last Update</th>\n";
    $temp .= "  </tr>\n";
    $translations = $xmlarray['TRANSLATIONS'][0]['TRANSLATION'];
    $translations = $page->multisortArray($translations, 'LANGUAGE', SORT_ASC);
    for ($i = 0; $i < count($translations); $i++) { //For all translations...
      $translation = $translations[$i];
      $language_name = $translation['LANGUAGE'][0]['VALUE'];
      if ($language_name != 'English') { //If NOT English...
        $file_name = $translation['FILE'][0]['VALUE'];
        $translation_update = $translation['UPDATE'][0]['VALUE'];
        $strings = $translation['STRINGS'][0];
        $strings_count = $strings['COUNT'][0]['VALUE'];
        $strings_translated = $strings['TRANSLATED'][0]['VALUE'];
        $strings_fuzzy = $strings['FUZZY'][0]['VALUE'];
        $strings_untranslated = $strings['UNTRANSLATED'][0]['VALUE'];
        
        $temp .= "  <tr>\n";
        $temp .= "    <td class=\"left\"><a href=\"" . $svnurl . $file_name . "\" rel=\"nofollow\">" . $language_name . "</a></td>\n";
        $temp .= "    <td class=\"center\">" . get_status_graph($strings_count, $strings_translated, $strings_fuzzy, $strings_untranslated) . "</td>\n";
        $temp .= "    <td class=\"right translated\">" . $strings_translated . "</td>\n";
        $temp .= "    <td class=\"right fuzzy\">" . $strings_fuzzy . "</td>\n";
        $temp .= "    <td class=\"right untranslated\">" . $strings_untranslated . "</td>\n";
        $temp .= "    <td class=\"center\">" . $translation_update . "</td>\n";
        $temp .= "  </tr>\n";
      }
      else { //If English...
        $english = $translations[$i];
        $english_file_name = $english['FILE'][0]['VALUE'];
        $english_translation_update = $english['UPDATE'][0]['VALUE'];
        $english_strings_count = $english['STRINGS'][0]['COUNT'][0]['VALUE'];
      }
    }
    $temp .= "  <tr>\n";
    $temp .= "    <td class=\"left\"><a href=\"" . $svnurl . $english_file_name . "\" rel=\"nofollow\">English (Template)</a></td>\n";
    $temp .= "    <td class=\"center\"></td>\n";
    $temp .= "    <td colspan=\"3\" class=\"center\">Total: " . $english_strings_count . "</td>\n";
    $temp .= "    <td class=\"center\">" . $english_translation_update . "</td>\n";
    $temp .= "  </tr>\n";
    $temp .= "</table>\n";
    print($temp);
  }
  
  /**
   * ...
   */
  function printTranslationsStatusGraphLegend() {
    $temp = "<ul class=\"inline\">\n";
    $temp .= "  <li><img src=\"images/translated.png\" alt=\"*\" width=\"16\" height=\"16\"> Translated</li>\n";
    $temp .= "  <li><img src=\"images/fuzzy.png\" alt=\"*\" width=\"16\" height=\"16\"> Fuzzy</li>\n";
    $temp .= "  <li><img src=\"images/untranslated.png\" alt=\"*\" width=\"16\" height=\"16\"> Untranslated</li>\n";
    $temp .= "</ul>\n";
    print($temp);
  }
  
  /**
   * ...
   */
  function printTranslationsStatusRSS($xmlarray, $version, $svnurl) {
    global $page;
    
    switch ($version) {
      case 'Trunk': $version_title = 'Unstable Trunk'; break;
      case 'Branch': $version_title = 'Stable Branch'; break;
      default: $version_title = $version;
    }
    
    $update = $xmlarray['TRANSLATIONS'][0]['UPDATE'][0]['VALUE'];
    $temp = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
    $temp .= "<rss version=\"2.0\">\n";
    $temp .= "  <channel>\n";
    $temp .= "    <title>WinMerge Translations Status (" . $version_title . ")</title>\n";
    $temp .= "    <link>http://winmerge.org/</link>\n";
    $temp .= "    <description>...</description>\n";
    $temp .= "    <item>\n";
    $temp .= "      <title>Status from " . $update . "</title>\n";
    $temp .= "      <description>\n";
    $temp .= "        <![CDATA[\n";
    
    $temp .= "<ul>\n";
    $translations = $xmlarray['TRANSLATIONS'][0]['TRANSLATION'];
    $translations = $page->multisortArray($translations, 'LANGUAGE', SORT_ASC);
    for ($i = 0; $i < count($translations); $i++) { //For all translations...
      $translation = $translations[$i];
      $language_name = $translation['LANGUAGE'][0]['VALUE'];
      if ($language_name != 'English') { //If NOT English...
        $file_name = $translation['FILE'][0]['VALUE'];
        $translation_update = $translation['UPDATE'][0]['VALUE'];
        $strings = $translation['STRINGS'][0];
        $strings_untranslated = $strings['UNTRANSLATED'][0]['VALUE'];
        
        $temp .= "  <li>\n";
        $temp .= "    <a href=\"" . $svnurl . $file_name . "\">" . $language_name . "</a> (" . $strings_untranslated . " Untranslated)\n";
        $temp .= "  </li>\n";
      }
    }
    $temp .= "</ul>\n";
    
    $temp .= "        ]]>\n";
    $temp .= "      </description>\n";
    $temp .= "      <guid>" . $update . "</guid>\n";
    $temp .= "      <link>http://" . $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF'] . "</link>\n";
    $temp .= "    </item>\n";
    $temp .= "  </channel>\n";
    $temp .= "</rss>\n";
    print($temp);
  }
  
  /**
   * ...
   */
  function printTranslationsTranslators($xmlarray) {
    global $page;
    
    $temp = "<table class=\"status\">\n";
    $temp .= "  <tr>\n";
    $temp .= "    <th class=\"left\">Language</th>\n";
    $temp .= "    <th class=\"left\">Translator(s)</th>\n";
    $temp .= "  </tr>\n";
    $translations = $xmlarray['TRANSLATIONS'][0]['TRANSLATION'];
    $translations = $page->multisortArray($translations, 'LANGUAGE', SORT_ASC);
    for ($i = 0; $i < count($translations); $i++) { //For all translations...
      $translation = $translations[$i];
      $language_name = $translation['LANGUAGE'][0]['VALUE'];
      if ($language_name != 'English') { //If NOT English...
        $translators = @$translation['TRANSLATORS'][0]['TRANSLATOR'];
        
        $temp .= "  <tr>\n";
        $temp .= "    <td class=\"left\">" . $language_name . "</td>\n";
        $temp .= "    <td class=\"left\">";
        for ($j = 0; $j < count($translators); $j++) { //For all translators...
          $translator = $translators[$j];
          $translator_name = utf8_encode(@$translator['NAME'][0]['VALUE']);
          $translator_mail = utf8_encode(@$translator['MAIL'][0]['VALUE']);
          $maintainer = @$translator['ATTRIBUTES']['MAINTAINER'];
          
          if ($maintainer == '1')
            $temp .= "<strong title=\"Maintainer\">";
          if ($translator_mail != '')
            $temp .= "<a href=\"mailto:" . $translator_mail . "\" title=\"" . htmlentities($translator_name, ENT_COMPAT, 'UTF-8') . " &lt;" . $translator_mail . "&gt;\">" . htmlentities($translator_name, ENT_COMPAT, 'UTF-8') . "</a>";
          else
            $temp .= $translator_name;
          if ($maintainer == '1')
            $temp .= "</strong>";
          $temp .= "<br>";
        }
        $temp .= "</td>\n";
        $temp .= "  </tr>\n";
      }
    }
    $temp .= "</table>\n";
    print($temp);
  }
?>