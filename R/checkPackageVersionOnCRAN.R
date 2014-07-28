#' checks what is the version of package on CRAN
#'
#' Checks what is the version of package on CRAN.
#' The CRAN page (baseUrlCRAN+packageName) is checked
#' and parsed extracting the line with
#' "Package source:      packageName_Version.tar.gz"
#' e.g.
#' "Package source:      GenABEL_1.6-9.tar.gz"
#' and then the 'Version' is returned.
#' Otherwise, NULL is returned.
#'
#' @return string containing CRAN version
#' of the package
#'
#' @param packageName name of the package to check
#'
#' @examples
#' library(GenABEL)
#' packageVersion("GenABEL")
#' checkPackageVersionOnCRAN("GenABEL")
#'
#' @author Yurii Aulchenko
#'
checkPackageVersionOnCRAN <- function(packageName)
{


mirror_df <- getCRANmirrors(all = FALSE, local.only = FALSE)
mirror <- mirror_df$URL[mirror_df$Country == "Netherlands" & mirror_df$City == "Amsterdam"]


CRAN_mirror_ref <- paste(mirror, "/src/contrib", sep="")

all_cran_packs <- available.packages(contriburl=CRAN_mirror_ref)
all_cran_packs_df <- as.data.frame(all_cran_packs)

databel_info <- all_cran_packs_df[all_cran_packs_df$Package == "DatABEL",]
cranVersion <- as.character(databel_info$Version)

return(cranVersion)



# Below is an old verion which depends on CRAN site content that is unreliable.

#	# change default timout
#	svtmo <- options("timeout")
#	options("timeout"=timeout)
#	# page to check is
#	pageAddress <- paste(baseUrlCRAN,packageName,sep="/")
#	# establish connection to the CRAN page of the package
#	suppressWarnings(
#			conn <- try( url(pageAddress) , silent=TRUE )
#	)
#	# if connection ok, read full page, store the results in pageContent; if failed, pageContent <- "try-error"
#	if ( all( class(conn) != "try-error") ) {
#		suppressWarnings(
#				pageContent <- try( readLines(conn) , silent=TRUE )
#		)
#		close(conn)
#	} else {
#		pageContent <- "try-error"
#		class(pageContent) <- "try-error"
#	}
#	# restore default timeout
#	options("timeout"=svtmo)
#	# if failed in reading (pageContent is "try-error"), return NULL
#	if (class(pageContent) == "try-error") return(NULL)
#	# parse the page and get string starting with "Package source:"
#	targetLine <- pageContent[grep("source:",pageContent)]
#	# split the string at "Package_" and ".tar.gz"; the element before the last will contain the version
#	splitPattern <- paste(packageName,"_|.tar.gz",sep="")
#	stringSplit <- strsplit(targetLine,splitPattern)
#	cranVersion <- stringSplit[[1]][length(stringSplit[[1]])-1]
#	# return version
#  return(cranVersion)



# Below is a patch received from Marco De Simone <marco.desimone@cbm.fvg.it>. This version is, however, use content if CRAN that is aso unreliable.
#
#options("timeout"=svtmo)
## if failed in reading (pageContent is "try-error"), return NULL
#if (class(pageContent) == "try-error") return(NULL)
## parse the table inside the page for the cell with the content "Version:" and select the following cell that contains the version"
#targetLine <- pageContent[grep("Version:",pageContent)+1]
## Strip the html tags <td> and </td> and get the content value of version
#splitPattern <- paste("<td>|</td>",sep="")
#
#cranVersion <- stringSplit[[1]][length(stringSplit[[1]])]
## return version
#return(cranVersion)

}
