// config/passport.js

// load all the things we need
var LocalStrategy   = require('passport-local').Strategy;

// load up the user model
var User            = require('../app/models/user');

// expose this function to our app using module.exports
module.exports = function(passport) {

    // =========================================================================
    // passport session setup ==================================================
    // =========================================================================
    // required for persistent login sessions
    // passport needs ability to serialize and unserialize users out of session

    // used to serialize the user for the session
    passport.serializeUser(function(user, done) {
        done(null, user.user_id);
    });

    // used to deserialize the user
    passport.deserializeUser(function(user_id, done) {
        User.findById(user_id).then(function(user) {
            done(null, user);
        }).catch(function(err){

            if(err) throw err;
        });
    });

    // =========================================================================
    // LOCAL SIGNUP ============================================================
    // =========================================================================
    // we are using named strategies since we have one for login and one for signup
    // by default, if there was no name, it would just be called 'local'

    passport.use('local-signup', new LocalStrategy({
        // by default, local strategy uses username and password, we will override with email
        usernameField : 'email',
        passwordField : 'password',
        passReqToCallback : true // allows us to pass back the entire request to the callback
    },
    function(req, email, password, done) {

        // asynchronous
        // User.findOne wont fire unless data is sent back
        process.nextTick(function() {

        // find a user whose email is the same as the forms email
        // we are checking to see if the user trying to login already exists
        User.findOne({ where: { 'email' :  email }}).then(function(user) {

            // check to see if theres already a user with that email
            if(user){
                return done(null, false, req.flash('signupMessage', 'Email pris.'));
            }else{

                if(password == req.body.conf_password){

                    User.generateHash(password, function(err, hash) {
                    
                        // if there is no user with that email
                        // create the user
                        var newUser = User.build({

                            email: email,
                            password: hash
                        })
                        .save()
                        .then(function(newUser) {
                            
                            return done(null, newUser);
                        })
                        .catch(function(err){

                            return done(err, false);
                        });
                    });

                    
                       
                }
                else{

                    return done(null, false, req.flash('signupMessage', 'Mots de passe non identique'));
                }
               
            }

        }).catch(function(err){
            return done(err, false);
        });    

        });

    }));

    // =========================================================================
    // LOCAL LOGIN =============================================================
    // =========================================================================
    // we are using named strategies since we have one for login and one for signup
    // by default, if there was no name, it would just be called 'local'

    passport.use('local-login', new LocalStrategy({
        // by default, local strategy uses username and password, we will override with email
        usernameField : 'email',
        passwordField : 'password',
        passReqToCallback : true // allows us to pass back the entire request to the callback
    },
    function(req, email, password, done) { // callback with email and password from our form

        // find a user whose email is the same as the forms email
        // we are checking to see if the user trying to login already exists
        User.findOne({where:{ 'email' :  email }}).then(function(user) {

            // if no user is found, return the message
            if (!user)
                return done(null, false, req.flash('loginMessage', 'Utilisateur introuvable.')); // req.flash is the way to set flashdata using connect-flash


            if( user.validPassword(password) == false ){

                return done(null, false, req.flash('loginMessage', 'Mot de passe incorrect.')); 
                // create the loginMessage and save it to session as flashdata
            }
            else{

                // all is well, return successful user
                return done(null, user);
            }
            

        })
        .catch(function(err){

            return done(err, false);
        });

    }));

};

