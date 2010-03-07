#
# databel_filtered_R class internal util
#

databel_filtered_R_check <- function(x,force_connected=TRUE) 
{
	connected <- databel_base_R_check(x,force_connected=force_connected);
	return(connected)
}
