jQuery( document ).ready(function() {

    uwp_wizard_check_plugins();

    jQuery('.uwp_install_plugins').click(function() {
        uwp_wizard_check_plugins();
    });

});

function uwp_wizard_install_plugin( $slug,$nonce ) {

    var data = {
        'action':           'install-plugin',
        '_ajax_nonce':       $nonce,
        'slug':              $slug
    };

    jQuery.ajax({
        type: "POST",
        url: uwp_wizard_obj.ajaxurl,
        data: data,
        beforeSend: function() {
            jQuery( "."+$slug + " .uwp-plugin-status").html(jQuery('#uwp-installing-text').val());
        },
        success: function(data) {
            console.log(data);
            if(data.success){
                jQuery( "."+$slug + " .uwp-plugin-status").html(jQuery('#uwp-installed-text').val());
                jQuery( "."+$slug + " input:checkbox").removeClass('uwp_install_plugins').prop("disabled", true);
                uwp_wizard_check_plugins();
                uwp_wizard_install_plugins($nonce);
                if(data.data.activateUrl){
                    uwp_wizard_activate_plugin(data.data.activateUrl,$slug);
                }
            }else{
                alert('something went wrong');
            }
        }
    });

}

function uwp_wizard_activate_plugin($url,$slug){
    jQuery.post($url, function(data, status){
        console.log($slug+'plugin activated')
    });
}

function uwp_wizard_install_plugins($nonce){
    var $result = '';
    jQuery('.uwp_install_plugins').each(function() {
        if(this.checked){
            $result = uwp_wizard_install_plugin(this.id,$nonce);
            jQuery('.uwp-install-recommend').prop("disabled", true);
            return false;
        }
    });
}

function uwp_wizard_check_plugins(){
    var $install = '';
    jQuery('.uwp_install_plugins').each(function() {
        $install += this.checked ? "1," : "";
    });

    if($install){
        jQuery('.uwp-install-recommend').show();
        jQuery('.uwp-continue-recommend').hide();
    }else{
        jQuery('.uwp-install-recommend').hide();
        jQuery('.uwp-continue-recommend').show();
    }
}

function uwp_wizard_setup_menu($security){

    var $menu_id = jQuery( "#uwp_wizard_menu_id" ).val();
    var $menu_location = jQuery( "#uwp_wizard_menu_location" ).val();

    var data = {
        'action':           'uwp_wizard_setup_menu',
        'security':          $security,
        'menu_id':           $menu_id,
        'menu_location':     $menu_location
    };

    jQuery.ajax({
        type: "POST",
        url: uwp_wizard_obj.ajaxurl,
        data: data,
        beforeSend: function() {
            jQuery( ".uwp-wizard-menu-result" ).html('<i class="fas fa-sync fa-spin" style="font-size:18px"></i>');
        },
        success: function(data) {
            if(data.data){
                jQuery( ".uwp-wizard-menu-result" ).text(data.data);
            }
        }
    });

    return false;
}

function uwp_wizard_setup_dummy_users($security, type) {
    jQuery('.uwp_dummy_users_button').hide();
    jQuery("#uwp_diagnose_add_dummy_users,#uwp_diagnose_remove_dummy_users").html('');
    jQuery("#uwp_diagnose_pb_" + type).find('.progressBar').show().progressbar({value: 0});
    uwp_wizard_process_diagnose_step( 0, type,$security );
}

function uwp_wizard_process_diagnose_step(step, type, security) {
    jQuery.ajax({
        url: uwp_wizard_obj.ajaxurl,
        type: 'POST',
        dataType: 'json',
        data: {
            action: 'uwp_process_diagnosis',
            step: step,
            type: type,
            security: security,
        },
        beforeSend: function() {},
        success: function(response, textStatus, xhr) {
            if(response.done === true || response.error === true ) {
                tools_progress(response.percent, type);
                setTimeout(function(){
                    jQuery("#uwp_diagnose_pb_" + type).find('.progressBar').hide();
                    jQuery("#uwp_diagnose_" + type).html(response.message);

                    if( 'add_dummy_users' === type ) {
                        jQuery('.uwp_remove_dummy_users_button').show();
                        jQuery('.uwp_add_dummy_users_button').hide();
                    } else{
                        jQuery('.uwp_add_dummy_users_button').show();
                        jQuery('.uwp_remove_dummy_users_button').hide();
                    }
                }, 1500);
            } else {
                setTimeout(function(){
                    tools_progress(response.percent, type);
                    uwp_wizard_process_diagnose_step(parseInt( response.step ), type,security)
                }, 500);
            }
        },
        error: function(xhr, textStatus, errorThrown) {
            alert(textStatus);
        }
    });
}

function tools_progress(percent, type) {
    $element = jQuery("#uwp_diagnose_pb_" + type).find('.progressBar');
    var progressBarWidth = percent * $element.width() / 100;
    $element.find('div').animate({ width: progressBarWidth }, 500).html(percent + "% ");
}