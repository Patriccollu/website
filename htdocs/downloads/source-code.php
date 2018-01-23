<?php
  include('../engine/engine.inc');
  include('../engine/simplepie/simplepie.inc');

  $page = new Page;
  $page->setDescription(__('Download the source code of WinMerge, which is released under the GNU General Public License.'));
  $page->setKeywords(__('WinMerge, free, download, source code, GPL, Bitbucket, Mercurial'));
  $page->printHead(__('Download Source Code'), TAB_DOWNLOADS, 'toggle(\'checksumslist\');');
  $stablerelease = $page->getStableRelease();
  
  $page->printHeading(__('Download Source Code'));
  $page->printPara(__('WinMerge is released under the <a href="%s">GNU General Public License</a>. That means you can get the whole source code and can build the program yourself.', 'http://www.gnu.org/licenses/gpl-2.0.html'));
?>
<div class="downloadmatrix">
<ul>
  <li><strong><?php __e('WinMerge %s - Source Code', $stablerelease->getVersionNumber());?></strong>
    <ul>
      <li><a href="<?php echo $stablerelease->getDownload('src.zip');?>"><?php __e('Zip-Format (%s MB)', $stablerelease->getDownloadSizeMb('src.zip'));?></a></li>
      <li><a href="<?php echo $stablerelease->getDownload('src.7z');?>"><?php __e('7z-Format (%s MB)', $stablerelease->getDownloadSizeMb('src.7z'));?></a></li>
    </ul>
  </li>
</ul>
</div> <!-- .downloadmatrix -->
<div id="checksums">
  <h3><a href="javascript:toggle('checksumslist')"><?php __e('SHA-1 Checksums');?></a></h3>
<pre id="checksumslist">
<?php
  echo $stablerelease->getDownloadSha1Sum('src.zip') . ' ' . $stablerelease->getDownloadFileName('src.zip') . "\n";
  echo $stablerelease->getDownloadSha1Sum('src.7z') . ' ' . $stablerelease->getDownloadFileName('src.7z') . "\n";
?>
</pre>
</div> <!-- #checksums -->
<?php
  $page->printPara(__('The source code is hosted on <a href="%1$s">Bitbucket</a> in a <a href="%2$s">Mercurial</a> repository.', 'https://bitbucket.org/', 'https://www.mercurial-scm.org/'));
?>
<dl class="headinglist">
  <dt><?php __e('Developer Version');?></dt>
  <dd><a href="https://bitbucket.org/winmerge/winmerge">https://bitbucket.org/winmerge/winmerge</a></dd>
  <dt><?php __e('WinMerge %s', $stablerelease->getVersionNumberMajor());?></dt>
  <dd><a href="https://bitbucket.org/winmerge/winmerge/branch/<?php echo $stablerelease->getBranchName();?>">https://bitbucket.org/winmerge/winmerge/branch/<?php echo $stablerelease->getBranchName();?></a></dd>
</dl>
<?php
  $page->printFoot();
?>
